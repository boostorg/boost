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
# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std {
      using ::exit; using ::system; using ::strftime; using ::gmtime;
      using ::time; using ::time_t;
    }
# endif

std::string get_host()
{
#if defined __linux__
  return "linux";
#elif defined _WIN32
  return "win32";
#elif defined __BEOS__
  return "beos";
#else
# error Please adapt for your platform
#endif
}


// retrieve precise system configuration as descriptive string
#ifdef __unix__

#include <sys/utsname.h>

#define STRINGIFY(x) #x

std::string get_system_configuration()
{
  utsname ut;
  if(uname(&ut) < 0)
    return "";

  std::string config = std::string(ut.sysname) + " " + ut.release;
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
  do { std::getline( is, s ); }
    while ( is.good()
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
		const std::string & file)
{
  std::string exename = "boosttmp.exe";
  replace(command, "%source", boostpath + "/" + file);
  if(execute(command)) {
    return execute( (get_host() == "win32" ? "" : "./") + exename ) ?
      run_ok : run_failed;
  } else {
    return link_failed;
  }
}

std::pair<test_result, test_result>
run_test(const std::string & type, std::string compile_only_command,
   std::string compile_link_command,
	 const std::string & boostpath, const std::string & source)
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
    return std::make_pair(run(compile_link_command, boostpath, source), run_ok);
  else if(type == "run-fail")
    return std::make_pair(run(compile_link_command, boostpath, source), run_failed);
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
    std::string file(l, p+1, std::string::npos);  // 3rd arg to fix VC++ bug

    std::cout << "*** " << file << " ***\n\n";

    out << "<tr>\n"
        << "<td><a href=\"../" << file << "\">" << file << "</a></td>\n"
        << "<td>" << type << "</td>\n";

    for(ForwardIterator it = firstcompiler; it != lastcompiler; ++it) {
      std::cout << "** " << it->name << "\n";
      std::pair<test_result, test_result> result =
        run_test(type, it->compile_only_command, it->compile_link_command, boostpath, file);
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
//  std::vector<std::string> args(argv+1, argv+argc);
// hack around VC++ lack of ctor taking iterator args
  std::vector<std::string> args;
  for ( const char ** ait = (const char **)(argv+1);
        ait != (const char **)(argv+argc); ++ait)
    args.push_back(std::string( *ait ));

  if(args.size() < 3) {
    std::cerr << argv[0]
              << " usage: compiler-config test-config boost-path [compiler|* [[command] file]]\n";
    std::exit(1);
  }
  std::string compiler = (args.size() >= 4 ? args[3] : "*");
    
  std::list<entry> l;
  read_compiler_configuration(args[0], std::back_inserter(l));
  std::string host = get_host();
  for(std::list<entry>::iterator it = l.begin(); it != l.end(); ) {
    if(it->os == host && (compiler == "*" || it->identifier == compiler)) {
      replace_environment(it->compile_only_command);
      replace_environment(it->compile_link_command);
      ++it;
    } else {
      it = l.erase(it);
    }
  }

  std::string boostpath = args[2];

  // if file argument present, write temporary test configuration file for do_tests
  if(args.size() >= 5) { // file argument present 
    std::ofstream tmp((args[1]="boosttmp.tmp").c_str());
    if (args.size() >= 6) tmp << args[4] << " " << args[5] << std::endl; // command present
    else tmp << "compile " << args[4] << std::endl; // command not present
  }

  std::ofstream out( ("cs-" + host + ".html").c_str() );

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
    
  do_tests(out, l.begin(), l.end(), args[1], boostpath);

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
