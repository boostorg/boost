/* boost regression test program
 *
 * Copyright Jens Maurer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without free provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 *  2001-01-22  added --diff option (Jens Maurer)
 */

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <ctime>

// It is OK to use boost headers which contain entirely inline code.
#include <boost/config.hpp>
#ifdef BOOST_NO_STDC_NAMESPACE
    namespace std {
      using ::exit; using ::system; using ::strftime; using ::gmtime;
      using ::time; using ::time_t;
    }
#endif

std::string get_host()
{
#if defined __linux__
  return "linux";
#elif defined __osf__
  return "tru64";
#elif defined __sgi
  return "irix";
#elif defined __sun
  return "solaris";
#elif defined _WIN32
  return "win32";
#elif defined __BEOS__
  return "beos";
#else
# error Please adapt for your platform
#endif
}


// retrieve precise system configuration as descriptive string
#ifdef __unix

#include <sys/utsname.h>

std::string get_system_configuration()
{
  struct utsname ut;      // "struct" is required for the DEC Tru64 compiler
  if(uname(&ut) < 0)
    return "";

  std::string config = std::string(ut.sysname) + " " + ut.release;
  config = config + " (CPU: " + ut.machine + ")";
  return config;
}

#elif defined _WIN32

std::string get_system_configuration()
{
  return "Microsoft Windows 32bit";
}

#elif defined __BEOS__

std::string get_system_configuration()
{
  return "BeOS 5 Intel Edition";
}

#else
# error Please adapt for your platform
#endif


struct configuration
{
  std::string compiler_config_file, test_config_file;
  std::string boostpath;
  std::string html_output;
  bool note_differences;
  std::string compiler, test;

  // defaults
  configuration()
    : compiler_config_file("compiler.cfg"), test_config_file("regression.cfg"),
      boostpath(".."), html_output("cs-" + get_host() + ".html"),
      note_differences(false),
      compiler("*"), test("") { }
};

configuration parse_command_line(char **first, char **last)
{
  configuration cfg;
  bool output_redirected = false;
  for( ; first != last; ++first) {
    std::string arg = *first;
    if(arg == "--config") {
      cfg.compiler_config_file = *++first;
    } else if(arg == "--tests") {
      cfg.test_config_file = *++first;
    } else if(arg == "--boost") {
      cfg.boostpath = *++first;
    } else if(arg == "-o" || arg == "--output") {
      cfg.html_output = *++first;
      output_redirected = true;
    } else if(arg == "--diff") {
      cfg.note_differences = true;
    } else if(arg == "--compiler") {
      cfg.compiler = *++first;
    } else if(arg.substr(0,1) == "-") {
      std::cerr << "usage:  regression [-h | --help] [--config compiler.cfg]\n"
		<< "    [--tests regression.cfg] [--boost path] [-o output.html] [--compiler <name>]\n"
		<< "    [test]\n"
		<< "  -h or --help    print this help message\n"
		<< "  --config <file> compiler configuration file (default: compiler.cfg)\n"
		<< "  --tests <file>  test configuration file (default: regression.cfg)\n"
		<< "  --boost <path>  filesystem path to main boost directory (default: ..)\n"
		<< "  -o <file>       name of output file (default: cs-OS.html)\n"
		<< "  --diff          highlight differences in output file\n"
		<< "  --compiler <name>  use only compiler <name> (default: *)\n"
		<< "  test            a single test, including the action (compile, run, etc.)\n";
      std::exit(1);
    } else {
      // add the rest of the command line to the "test" line
      for( ; first != last; ++first)
	cfg.test += std::string(*first) + " ";
      break;
    }
  }
  if(cfg.test != "" && !output_redirected) {
    std::cerr << "Error: Please specify the HTML output file explicitly\n"
	      << "(using \"--output file\") when running a single test only.\n";
    std::exit(1);
  }
  return cfg;
}


struct entry
{
  std::string os, identifier, name, compile_only_command, compile_link_command, html;
};

// replace the first %name in s with value
void replace(std::string & s,
	     const std::string & name, const std::string & value)
{
  std::string::size_type p = s.find(name);
  if(p != std::string::npos)
    s.replace(p, name.length(), value);
}

// replace all $XXX in s with the value of getenv("XXX")
void replace_environment(std::string & s)
{
  std::string::size_type end = 0;
  for(;;) {
    std::string::size_type pos = s.find('$', end);
    if(pos == std::string::npos)
      break;
    end = s.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", pos+1);
    const char * env = getenv(s.substr(pos+1, end-pos-1).c_str());
    if(env)
      replace(s, s.substr(pos, end-pos), env);
    else
      break;
  }
}

// get a string line, ignoring empty lines and comment lines
void getstringline( std::ifstream & is, std::string & s )
{
  do {
    std::getline( static_cast<std::istream&>(is), s ); // cast required by IRIX
  } while ( is.good()
	    && (!s.size() || (s.size() >= 2 && s[0] == '/' && s[1] == '/')) );
}

// read the compiler configuration from file and push entry's to out
template<class OutputIterator>
void read_compiler_configuration(const std::string & file, OutputIterator out)
{
  std::ifstream f(file.c_str());
  int lineno = 0;
  while(f.good()) {
    entry e;
    getstringline(f, e.os);
    getstringline(f, e.identifier);
    getstringline(f, e.name);
    getstringline(f, e.compile_only_command);
    getstringline(f, e.compile_link_command);
    getstringline(f, e.html);
    *out = e;
    ++out;
    std::string l;
    std::getline(f, l);
    lineno += 6;
    if(l != "") {
      std::cerr << file << ", line " << lineno
		<< ": Empty line expected, got " << l << "\n";
      std::exit(1);
    }
  }
}

const std::string pass_string = "Pass";
const std::string fail_string = "<font color=\"#FF0000\">Fail</font>";

// map test name to results, one character ("P" or "F") for each compiler
typedef std::map<std::string, std::string> previous_results_type;

previous_results_type read_previous_results(std::istream & is)
{
  previous_results_type result;
  // finite state machine
  enum { prefix, testname, command, testresult } status = prefix;
  std::string line, current_test;
  while(std::getline(is, line)) {
    if(status == prefix) {
      if(line.substr(0, 6) == "<table")
	status = testname;
    } else if(status == testname) {
      if(line.substr(0, 6) == "<td><a") {
	std::string::size_type pos = line.find(">", 5);
	if(pos == std::string::npos || pos+1 >= line.size()) {
	  std::cerr << "Line '" << line << "' has unknown format.";
	  continue;
	}
	std::string::size_type pos_end = line.find("<", pos);
	if(pos_end == std::string::npos) {
	  std::cerr << "Line '" << line << "' has unknown format.";
	  continue;
	}	  
	current_test = line.substr(pos+1, pos_end - (pos+1));
	status = command;
      } else if(line.substr(0, 8) == "</table>") {
	break;
      }
    } else if(status == command) {
      status = testresult;
    } else if(status == testresult) {
      if(line == "</tr>")
	status = testname;
      else if(line.find(pass_string) != std::string::npos)
	result[current_test].append("P");
      else if(line.find(fail_string) != std::string::npos)
	result[current_test].append("F");
      else
	std::cerr << "Line '" << line << "' has unknown format.";
    }
  }
  return result;
}

// run command (possibly needs portability adjustment)
bool execute(const std::string & command)
{
  std::cout << command << std::endl; // fix: endl ensures cout ordering
  return std::system(command.c_str()) == 0;
}

enum test_result {
  ok = 0,
  unknown_type,
  compile_failed, compile_ok, link_failed, link_ok, run_failed, run_ok
};

test_result compile(std::string command, const std::string & boostpath,
	     const std::string & file)
{
  replace(command, "%source", boostpath + "/" + file);
  return execute(command) ? compile_ok : compile_failed;
}

test_result link(std::string command, const std::string & boostpath,
		 const std::string & file)
{
  replace(command, "%source", boostpath + "/" + file);
  return execute(command) ? link_ok : link_failed;
}

test_result run(std::string command, const std::string & boostpath,
		const std::string & file, std::string args)
{
  std::string exename = "boosttmp.exe";
  replace(command, "%source", boostpath + "/" + file);
  if(execute(command)) {
    if(get_host() != "win32")
      exename = "./" + exename;
    replace(args, "%boost", boostpath);
    return execute(exename + " " + args) ? run_ok : run_failed;
  } else {
    return link_failed;
  }
}

std::pair<test_result, test_result>
run_test(const std::string & type, std::string compile_only_command,
	 std::string compile_link_command,
	 const std::string & boostpath, const std::string & source,
	 const std::string & args)
{
  replace(compile_only_command, "%include", boostpath);
  replace(compile_link_command, "%include", boostpath);
  if(type == "compile")
    return std::make_pair(compile(compile_only_command, boostpath, source), compile_ok);
  else if(type == "compile-fail")
    return std::make_pair(compile(compile_only_command, boostpath, source), compile_failed);
  else if(type == "link")
    return std::make_pair(link(compile_link_command, boostpath, source), link_ok);
  else if(type == "link-fail")
    return std::make_pair(link(compile_link_command, boostpath, source), link_failed);
  else if(type == "run")
    return std::make_pair(run(compile_link_command, boostpath, source, args), run_ok);
  else if(type == "run-fail")
    return std::make_pair(run(compile_link_command, boostpath, source, args), run_failed);
  else
    return std::make_pair(unknown_type, ok);
}

template<class ForwardIterator>
void do_tests(std::ostream & out,
	      ForwardIterator firstcompiler, ForwardIterator lastcompiler,
	      const std::string & testconfig, const std::string & boostpath,
	      const previous_results_type& previous_results)
{
  out << "<tr>\n"
      << "<td>Program</td>\n"
      << "<td>Test<br>Type</td>\n";
  for(ForwardIterator it = firstcompiler; it != lastcompiler; ++it) {
    out << "<td>" << it->html << "</td>\n";
  }
  out << "</tr>\n";

  std::ifstream f(testconfig.c_str());
  while(f.good()) {
    std::string l;
    getstringline(f, l);
    if (!f.good()) break;
    typedef std::string::size_type sz_type;
    sz_type p = l.find(' ');
    if(p == std::string::npos) {
      std::cerr << "Test " << l << " is wrong\n";
      continue;
    }
    std::string type(l, 0, p);
    sz_type end_filename = l.find(' ', p+1);
    std::string file, args;
    if(end_filename == std::string::npos) {
      file = l.substr(p+1, std::string::npos);
    } else {
      file = l.substr(p+1, end_filename-(p+1));
      args = l.substr(end_filename+1, std::string::npos);
    }

    std::cout << "*** " << file << " ***\n\n";

    out << "<tr>\n"
        << "<td><a href=\"../" << file << "\">" << file << "</a></td>\n"
        << "<td>" << type << "</td>\n";

    previous_results_type::const_iterator prev_iter =
      previous_results.find(file);
    std::string previous = (prev_iter == previous_results.end() ?
			    "" : prev_iter->second);
    std::string::size_type i = 0;

    for(ForwardIterator it = firstcompiler; it != lastcompiler; ++it, ++i) {
      std::cout << "** " << it->name << "\n";
      std::pair<test_result, test_result> result =
        run_test(type, it->compile_only_command, it->compile_link_command, boostpath, file, args);
      if(result.first == unknown_type) {
        std::cerr << "Unknown test type " << type << ", skipped\n";
        continue;
      }
      bool pass = result.first == result.second;
      char prev = (i < previous.size() ? previous[i] : ' ');
      bool changed = (prev == 'F' && pass) || (prev == 'P' && !pass) || 
	prev == ' ';
      out << "<td>"
	  << (changed ? "<font size=\"+3\"><em>" : "")
          << (pass ? pass_string : fail_string)
	  << (changed ? "</em></font>" : "")
          << "</td>" << std::endl;
      std::cout << (result.first == result.second ? "Pass" : "Fail") << "\n\n";
    }
    out << "</tr>\n";
  }
}
	      

int main(int argc, char * argv[])
{
  configuration config = parse_command_line(argv+1, argv+argc);
    
  std::list<entry> l;
  read_compiler_configuration(config.compiler_config_file,
			      std::back_inserter(l));
  std::string host = get_host();
  for(std::list<entry>::iterator it = l.begin(); it != l.end(); ) {
    if(it->os == host && (config.compiler == "*" ||
			  it->identifier == config.compiler)) {
      replace_environment(it->compile_only_command);
      replace_environment(it->compile_link_command);
      ++it;
    } else {
      it = l.erase(it);
    }
  }

  // if explicit test requested, write temporary file for do_tests
  if(config.test != "") {
    std::ofstream tmp((config.test_config_file="boosttmp.tmp").c_str());
    tmp << config.test << std::endl;
  }

  previous_results_type previous_results;
  if(config.note_differences) {
    std::ifstream in(config.html_output.c_str());
    previous_results = read_previous_results(in);
  }

  std::ofstream out( config.html_output.c_str() );

  char run_date[100];
  std::time_t ct;
  std::time(&ct);
  std::strftime(run_date, sizeof(run_date), "%d %b %Y %H:%M GMT", std::gmtime(&ct)); 

  out << "<html>\n<head>\n<title>\nCompiler Status: " + host + "\n</title>\n</head>\n"
      << "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
      << "<h1><img border border=\"0\" src=\"../c++boost.gif\" width=\"277\" height=\"86\"></h1>\n"
      << "<h1>Compiler Status: " + host + "</h1>\n"
      << "\n"
      << "<p><b>Run Date:</b> " << run_date << "</p>\n"
      << "<p><b>System Configuration:</b> " << get_system_configuration()
      << "</p>\n"
      << "<p>\n" 
      << "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">\n";
    
  do_tests(out, l.begin(), l.end(), config.test_config_file, config.boostpath,
	   previous_results);

  out << "</table></p>\n<p>\n";
  if(host == "linux")
    out << "Notes: A hand-crafted &lt;limits&gt; Standard header has been\n"
	<< "applied to all configurations.\n"
	<< "The tests were run on a GNU libc 2.2 system which has improved\n"
	<< "wide character support compared to previous versions.";
  else if(host == "irix" || host == "tru64")
    out << "Note: For the 'clib' configuration, the missing new-style C\n"
	<< "library headers &lt;cXXX&gt; have been supplied.\n";

  out << "</p>\n</body>\n</html>" << std::endl;
  return 0;
}
