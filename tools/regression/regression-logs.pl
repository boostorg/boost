#!/usr/bin/perl
# Copyright (C) 2003, Rene Rivera. Permission to copy, use, modify, sell and
# distribute this software is granted provided this copyright notice appears in
# all copies. This software is provided "as is" without express or implied
# warranty, and with no claim as to its suitability for any purpose.

# This the cgi script that generates the live summary page for the regression
# logs located at http://boost.sourceforge.net/regression-logs
#
# Per SourceForge requirements this script needs to be located in the cgi-bin
# directory (/home/groups/b/bo/boost/cgi-bin) for it to be recognized as a script.
#
# This script only generates the summary table of the test. It doesn not generate
# the entire HTML page. For the complete page this script is called, with SSI, from
# the "index.shtml" page which contains the wrapping page. This makes it easier to
# mainting the table independently of the rest of the page.

use FileHandle;
use Time::Local;

print "Content-type: text/html\r\n\r\n";

# Generate an individual result item, Pass, Warn, and Fail columns.
# Use as: result_info(html-color,result-count,total-count)
#
sub result_info
{
	my ($color,$result,$total) = @_;
	my $percent = int (($result/$total)*100+0.5);
	return "<font color=\"$color\"><font size=\"+1\">$percent%</font><br>($result)</font>";
}

# Generate an age highlighted run date string.
# Use as: data_info(run-date-html)
#
sub date_info
{
	my %m = ('January',0,'February',1,'March',2,'April',3,'May',4,'June',5,
		'July',6,'August',7,'September',8,'October',9,'November',10,'December',11);
	my @d = split(/ |:/,$_[0]);
	my ($hour,$min,$sec,$day,$month,$year) = ($d[0],$d[1],$d[2],$d[4],$m{$d[5]},$d[6]);
	#print "<!-- $hour.$min.$sec.$day.$month.$year -->\n";
	my $test_t = timegm($sec,$min,$hour,$day,$month,$year);
	my $age = time-$test_t;
	my $age_days = $age/(60*60*24);
	#print "<!-- $age_days days old -->\n";
	my $age = "<font>";
	if ($age_days <= 2) { }
	elsif ($age_days <= 14) { $age = "<font color=\"#FF9900\">"; }
	else { $age = "<font color=\"#FF0000\">"; }
	return $age.$_[0]."</font>";
}

# Generate an age string based on the run date.
# Use as: age_info(run-date-html)
#
sub age_info
{
	my %m = ('January',0,'February',1,'March',2,'April',3,'May',4,'June',5,
		'July',6,'August',7,'September',8,'October',9,'November',10,'December',11);
	my @d = split(/ |:/,$_[0]);
	my ($hour,$min,$sec,$day,$month,$year) = ($d[0],$d[1],$d[2],$d[4],$m{$d[5]},$d[6]);
	#print "<!-- $hour.$min.$sec.$day.$month.$year -->\n";
	my $test_t = timegm($sec,$min,$hour,$day,$month,$year);
	my $age = time-$test_t;
	my $age_days = $age/(60*60*24);
	#print "<!-- $age_days days old -->\n";
	my $age = "<font>";
	if ($age_days <= 2) { }
	elsif ($age_days <= 14) { $age = "<font color=\"#FF9900\">"; }
	else { $age = "<font color=\"#FF0000\">"; }
	if ($age_days <= 1) { $age = $age."today"; }
	elsif ($age_days <= 2) { $age = $age."yesterday"; }
	elsif ($age_days < 14) { my $days = int $age_days; $age = $age.$days." days"; }
	elsif ($age_days < 7*8) { my $weeks = int $age_days/7; $age = $age.$weeks." weeks"; }
	else { my $months = int $age_days/28; $age = $age.$months." months"; }
	return $age."</font>";
}

opendir LOGS, "/home/groups/b/bo/boost/htdocs/regression-logs";
my @logs = grep /.*links[^.]*\.html$/, readdir LOGS;
closedir LOGS;
my @bgcolor = ( "bgcolor=\"#EEEEFF\"", "" );
my $row = 0;
print "<table>\n";
print "<tr>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Platform</th>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Run Date</th>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Age</th>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Compilers</th>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Pass</th>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Warn</th>\n",
	"<th align=\"left\" bgcolor=\"#DDDDDD\">Fail</th>\n",
	"</tr>\n";
foreach $l (sort { lc($a) cmp lc($b) } @logs)
{
	my $log = $l;
	$log =~ s/-links//s;
	my ($spec) = ($log =~ /cs-([^\.]+)/);
	my $fh = new FileHandle;
	if ($fh->open("</home/groups/b/bo/boost/htdocs/regression-logs/$log"))
	{
		my $content = join('',$fh->getlines());
		$fh->close;
		my ($status) = ($content =~ /(<h1>Compiler(.(?!<\/td>))+.)/si);
		my ($platform) = ($status =~ /Status: ([^<]+)/si);
		my ($run_date) = ($status =~ /Date:<\/b> ([^<]+)/si);
		$run_date =~ s/, /<br>/g;
		my ($compilers) = ($content =~ /Test Type<\/a><\/td>((.(?!<\/tr>))+.)/si);
		if ($compilers eq "") { next; }
		$compilers =~ s/-<br>//g;
		$compilers =~ s/<\/td>//g;
		my @compiler = ($compilers =~ /<td>(.*)$/gim);
		my $count = @compiler;
		my @results = ($content =~ /(>Pass<|>Warn<|>Fail<)/gi);
		my $test_count = (scalar @results)/$count;
		my @pass = map { 0 } (1..$count);
		my @warn = map { 0 } (1..$count);
		my @fail = map { 0 } (1..$count);
		my @total = map { 0 } (1..$count);
		for my $t (1..$test_count)
		{
			my @r = @results[(($t-1)*$count)..(($t-1)*$count+$count-1)];
			for my $c (1..$count)
			{
				if ($r[$c-1] =~ /Pass/i) { ++$pass[$c-1]; }
				elsif ($r[$c-1] =~ /Warn/i) { ++$warn[$c-1]; }
				elsif ($r[$c-1] =~ /Fail/i) { ++$fail[$c-1]; }
				++$total[$c-1];
			}
		}
		for my $comp (1..(scalar @compiler))
		{
			my @lines = split(/<br>/,$compiler[$comp-1]);
			if (@lines > 2) { $compiler[$comp-1] = join(' ',@lines[0..(scalar @lines)-2])."<br>".$lines[(scalar @lines)-1]; }
		}
		print
			"<tr>\n",
			"<td rowspan=\"$count\" valign=\"top\"><font size=\"+1\">$platform</font><br>(<a href=\"../regression-logs/$log\">$spec</a>)</td>\n",
			"<td rowspan=\"$count\" valign=\"top\">",$run_date,"</td>\n",
			"<td rowspan=\"$count\" valign=\"top\">",age_info($run_date),"</td>\n",
			"<td valign=\"top\" ",$bgcolor[$row],">",$compiler[0],"</td>\n",
			"<td valign=\"top\" ",$bgcolor[$row],">",result_info("#000000",$pass[0],$total[0]),"</td>\n",
			"<td valign=\"top\" ",$bgcolor[$row],">",result_info("#FF9900",$warn[0],$total[0]),"</td>\n",
			"<td valign=\"top\" ",$bgcolor[$row],">",result_info("#FF0000",$fail[0],$total[0]),"</td>\n",
			"</tr>\n";
		$row = ($row+1)%2;
		foreach my $c (1..($count-1))
		{
			print
				"<tr>\n",
				"<td valign=\"top\" ",$bgcolor[$row],">",$compiler[$c],"</td>\n",
				"<td valign=\"top\" ",$bgcolor[$row],">",result_info("#000000",$pass[$c],$total[$c]),"</td>\n",
				"<td valign=\"top\" ",$bgcolor[$row],">",result_info("#FF9900",$warn[$c],$total[$c]),"</td>\n",
				"<td valign=\"top\" ",$bgcolor[$row],">",result_info("#FF0000",$fail[$c],$total[$c]),"</td>\n",
				"</tr>\n";
			$row = ($row+1)%2;
		}
		print
			"<tr>\n",
			"<td colspan=\"7\"><hr size=\"1\" noshade></td>\n",
			"</tr>\n";
	}
}
print "</table>\n";
