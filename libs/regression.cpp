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


std::string get_host()
{
#if defined __linux__
  return "linux";
#elif defined _MSC_VER
  return "win32";
#else
#error Please adapt for your platform
#endif
}


struct entry
{
  std::string os, identifier, name, command, html;
};

void replace(std::string & s,
	     const std::string & name, const std::string & value)
{
  std::string::size_type p = s.find(name);
  if(p != std::string::npos)
    s.replace(p, name.length(), value);
}

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

template<class OutputIterator>
void read_compiler_configuration(const std::string & file, OutputIterator out)
{
  std::ifstream f(file.c_str());
  int lineno = 0;
  while(f.good()) {
    entry e;
    std::getline(f, e.os);
    std::getline(f, e.identifier);
    std::getline(f, e.name);
    std::getline(f, e.command);
    std::getline(f, e.html);
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

bool execute(const std::string & command)
{
  std::cout << command << "\n";
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
  replace(command, "%source", "-o boosttmp.o " + boostpath + "/" + file);
  return execute(command) ? compile_ok : compile_failed;
}

test_result link(std::string command, const std::string & boostpath,
		 const std::string & file)
{
  replace(command, "%source", "-o boosttmp.exe" + boostpath + "/" + file);
  return execute(command) ? link_ok : link_failed;
}

test_result run(std::string command, const std::string & boostpath,
		const std::string & file)
{
  std::string exename = "boosttmp.exe";
  replace(command, "%source", "-o " + exename + " " + boostpath + "/" + file);
  if(execute(command)) {
    return execute( (get_host() == "win32" ? "" : "./") + exename ) ?
      run_ok : run_failed;
  } else {
    return link_failed;
  }
}

std::pair<test_result, test_result>
run_test(const std::string & type, std::string command,
	 const std::string & boostpath, const std::string & source)
{
  replace(command, "%include", boostpath);
  if(type == "compile")
    return std::make_pair(compile(command, boostpath, source), compile_ok);
  else if(type == "compile-fail")
    return std::make_pair(compile(command, boostpath, source), compile_failed);
  else if(type == "link")
    return std::make_pair(link(command, boostpath, source), link_ok);
  else if(type == "link-fail")
    return std::make_pair(link(command, boostpath, source), link_failed);
  else if(type == "run")
    return std::make_pair(run(command, boostpath, source), run_ok);
  else if(type == "run-fail")
    return std::make_pair(run(command, boostpath, source), run_failed);
  else
    return std::make_pair(unknown_type, ok);
}

template<class ForwardIterator>
void do_tests(std::ostream & out,
	      ForwardIterator firstcompiler, ForwardIterator lastcompiler,
	      const std::string & testconfig, const std::string & boostpath)
{
  out << "<tr>\n"
      << "<td>Program</td>\n";
  for(ForwardIterator it = firstcompiler; it != lastcompiler; ++it) {
    out << "<td>" << it->html << "</td>\n";
  }
  out << "</tr>\n";

  std::ifstream f(testconfig.c_str());
  while(f.good()) {
    std::string l;
    std::getline(f, l);
    typedef std::string::size_type sz_type;
    sz_type p = l.find(' ');
    if(p == std::string::npos) {
      std::cerr << "Test " << l << " is wrong\n";
      continue;
    }
    std::string type(l, 0, p);
    std::string file(l, p+1);

    std::cout << file << "\n";

    out << "<tr>\n"
	<< "<td><a href=\"" << file << "\">" << file << "</a></td>\n";

    for(ForwardIterator it = firstcompiler; it != lastcompiler; ++it) {
      std::cout << "\t" << it->name << std::endl;
      std::pair<test_result, test_result> result =
	run_test(type, it->command, boostpath, file);
      if(result.first == unknown_type) {
	std::cerr << "Unknown test type " << type << ", skipped\n";
	continue;
      }
      out << "<td>";
      if(result.first == result.second)
	out << "yes";
      else
	out << "no";
      out << "<sup>" << int(result.second) << "</sup></td>" << std::endl;
      std::cout << (result.first == result.second ? "ok" : "failed") << "\n";
    }
    out << "</tr>\n";
  }
}
	      

int main(int argc, char * argv[])
{
  std::vector<std::string> args(argv+1, argv+argc);
  if(args.size() < 3) {
    std::cerr << argv[0] << " usage: compiler-config test-config boost-path [compiler] [file]\n";
    std::exit(1);
  }
  std::string compiler = (args.size() >= 4 ? args[3] : "*");
    
  std::list<entry> l;
  read_compiler_configuration(args[0], std::back_inserter(l));
  std::string host = get_host();
  for(std::list<entry>::iterator it = l.begin(); it != l.end(); ) {
    if(it->os == host && (compiler == "*" || it->identifier == compiler)) {
      replace_environment(it->command);
      ++it;
    } else {
      it = l.erase(it);
    }
  }

  std::string boostpath = args[2];

  if(args.size() >= 5) {
    std::string cmd = l.front().command;
    replace(cmd, "%include", boostpath);
    compile(cmd, boostpath, args[4]);
    return 0;
  }

  std::ofstream out( ("cs-" + host + ".html").c_str() );

  out << "<html>\n<head>\n<title>\nCompiler Status: " + host + "\n</title>\n</head>\n"
      << "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
      << "<h1><img border border=\"0\" src=\"../c++boost.gif\" width=\"277\" height=\"86\"></h1>\n"
      << "<h1>Compiler Status: " + host + "</h1>\n"
      << "</p>\n"
      << "<p>\n" 
      << "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">\n";
    
  do_tests(out, l.begin(), l.end(), args[1], boostpath);

  out << "</table>\n";
  out << "<p>\n"
      << "2: compile failed<br>\n"
      << "3: compile succeeded<br>\n"
      << "4: link failed<br>\n"
      << "5: link succeeded<br>\n"
      << "6: run failed<br>\n"
      << "7: run succeeded<br>\n";
  return 0;
}
