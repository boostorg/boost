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
 */

#include <iostream>
#include <string>
#include <list>
#include <vector>
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
  return "BeOS";
}

#else
# error Please adapt for your platform
#endif


struct configuration
{
  std::string compiler_config_file, test_config_file;
  std::string boostpath;
  std::string html_output;
  std::string compiler, test;

  // defaults
  configuration()
    : compiler_config_file("compiler.cfg"), test_config_file("regression.cfg"),
      boostpath(".."), html_output("cs-" + get_host() + ".html"),
      compiler("*"), test("") { }
};

configuration parse_command_line(char **first, char **last)
{
  configuration cfg;
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
		const std::string & file, const std::string & args)
{
  std::string exename = "boosttmp.exe";
  replace(command, "%source", boostpath + "/" + file);
  if(execute(command)) {
    if(get_host() != "win32")
      exename = "./" + exename;
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
	      const std::string & testconfig, const std::string & boostpath)
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
    std::string file(l, p+1, end_filename-(p+1));
    std::string args(l, end_filename+1, std::string::npos);

    std::cout << "*** " << file << " ***\n\n";

    out << "<tr>\n"
        << "<td><a href=\"../" << file << "\">" << file << "</a></td>\n"
        << "<td>" << type << "</td>\n";

    for(ForwardIterator it = firstcompiler; it != lastcompiler; ++it) {
      std::cout << "** " << it->name << "\n";
      std::pair<test_result, test_result> result =
        run_test(type, it->compile_only_command, it->compile_link_command, boostpath, file, args);
      if(result.first == unknown_type) {
        std::cerr << "Unknown test type " << type << ", skipped\n";
        continue;
      }
      out << "<td>"
          << (result.first == result.second ? "Pass" : "<font color=\"#FF0000\">Fail</font>")
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
    
  do_tests(out, l.begin(), l.end(), config.test_config_file, config.boostpath);

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
