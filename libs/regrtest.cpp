// boost compilation regression test

// Usage: regrtest [*|compiler] [*|library/program]
//
// Default: regrtest * *
//
// Compilers:  bcc   = Borland 5.5.1
//             cw    = Metrowerks CodeWarrior
//             gcc   = GNU GCC
//             gcc-stlport  = GNU GCC with STLport library
//             como  = Comeau C++
//             vc    = Microsoft Visual C++
//             vcstlport  = Microsoft Visual C++ with STLport library
//
// Examples: regrtest
//           regrtest
//           regrtest gcc
//           regrtest * smart_ptr/smart_ptr_test.cpp
//           regrtest gcc smart_ptr/smart_ptr_test.cpp
//
// If the program argument is * or left out, then the file
// ./regrtest_files.txt will be used as the list of files to be
// tested. Each line of regrtest_files.txt must have the form:
//
//   file-name mode [input-file]
//
// Where mode is
//   C  compile
//   F  compile, expecting failure
//   R  compile and run
//
// The path to the input-file should be relative to where regrtest is
// running.
//
// Required environment variables:
//    BOOST_PATH  The directory containing the "boost/" header file directory.
//    OS          The operating system, should be one of ...
//    BOOST_STLPORT_PATH  The directory containing STLport headers
//    BOOST_BCC55_PATH    The directory container Borland headers
//
// Note: use the following command line syntax if output is to be redirected:
//           python regrtest.py [*|compiler] [*|library/program]  >log 2>&1

// Revision history:
//  17 Dec 00 Rewrote in C++ and retrieve file list from a file.  (Jeremy Siek)
//  21 Jun 00 Redesign to allow specifying compiler and program (Beman Dawes)
//  18 Jun 00 Initial Version (Beman Dawes)

// The Metrowerks and Microsoft compilers require various environment variables be set.
//   See mwcc -help
//   See http://msdn.microsoft.com/library/devprods/vs6/visualc/vccore/_core_building_on_the_command_line.3a_.overview.htm
// Others:
//   See bcb4.hlp.  Don't bother with bcb4tools.hlp; it has a bad link to the command line options


#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h> // for getenv()
#include <time.h>   // for ctime()
#include <stdio.h>  // for sscanf()
#include <string.h> // for strtok()

// Enumerated Types

// Global variables

std::string path, compiler_arg, program_arg, exe_suffix, exe_invoke_prefix;

std::ofstream outfile;

//-----------------------------------------------------------------------------
std::string platform()
{
  char* os_ptr = getenv("OS");
  if (os_ptr == 0) {
    std::cerr << "The \"OS\" environment variable is not defined" 
              << std::endl;
    exit(1);
    return "unknown";
  } else {
    std::string os = os_ptr;
    if (os == "linux")
      return "linux";
    else if (os == "solaris2.7")
      return "sunos";
    else if (os == "Windows_NT")
      return "windows";
    else
      return "unknown";
  }
}

//-----------------------------------------------------------------------------

void invoke(std::string desc, 
            std::string command, 
            char invoke_mode,
            std::string invoke_args,
            std::string program_name)
{
  std::cout << " " << desc << std::endl;
  std::cout << " invoke mode: " << invoke_mode << std::endl;
  std::cout << " " << command << std::endl;

  outfile << "<td>";
  int rs = system(command.c_str());
  std::cout << " compile return status: " << rs << std::endl;

  switch (invoke_mode) {
  case 'C': // compile
    if (rs==0)
      outfile << "<FONT COLOR=#008000>yes</FONT>";
    else
      outfile << "<FONT COLOR=#800000>failed to compile</FONT>";
    break;
  case 'F': // compile, fail expected
    if (rs==0)
      outfile
        << "<FONT COLOR=#800000>failed to cause error</FONT>";
    else
      outfile
        << "<FONT COLOR=#008000>yes</FONT>";
    break;
  case 'R': // run
    if (rs==0) {
      //script debugging aid
      std::cout << "  executing: " << exe_invoke_prefix << program_name
                << " " << invoke_args << std::endl;
      std::string cmd_line = exe_invoke_prefix + program_name
        + " " + invoke_args;
      rs = system(cmd_line.c_str());
      if (rs==0)
        outfile
          << "<FONT COLOR=#008000>yes</FONT>";
      else
        outfile << "<FONT COLOR=#800000>exited with code "
                << rs << "</FONT>";
    } else
      outfile << "<FONT COLOR=#800000>failed to compile</FONT>";
    break;
  default:
    outfile << "scripting error";
  } // switch (invoke_mode)

  outfile << "</td>" << std::endl;
}

//-----------------------------------------------------------------------------

void compile(std::string program, 
             char invoke_mode,
             std::string invoke_args,
             std::string program_name)
{
  std::string fullpath = path +  "/libs/" + program;
  std::cout << std::endl 
            << "*****" << program << "*****" << std::endl;

  outfile << "<tr>" << std::endl
          << "<td><a href=\"" << program << "\">" << program << "</a></td>"
          << std::endl;

  std::string gcc_flags
    = "-Wall -pedantic -ftemplate-depth-30 -Wno-long-long";
  // should add -Werror

  std::string kcc_flags
    //    = "--strict";
    = "--strict_warnings";

  std::string mwcc_flags = "-maxerrors 10 -cwd source";

  //John Maddock says use /Zm400 switch; it increases compiler memory
  std::string msvc_flags = "/nologo /Zm400 /MDd /W3 /GR /GX /GZ /D \"WIN32\" /D \"_DEBUG\" /D \"_MBCS\" /D \"_CONSOLE\"";
  
  if (platform() == "linux") {
    if (compiler_arg == "*" || compiler_arg == "gcc")
      invoke("GCC 2.95.2", "g++ " + gcc_flags 
             + " -o " + program_name
             + " -I"  + path + " " + fullpath, 
             invoke_mode, invoke_args, program_name);

    if (compiler_arg == "*" || compiler_arg == "gcc-stlport")
      invoke( "GCC 2.95.2 STLport 4.0", 
              "g++ -V 2.95.2-stlport " + gcc_flags
              + " -o " + program_name 
              + " -I" + path + " " + fullpath, 
              invoke_mode, invoke_args, program_name );

    if (compiler_arg == "*" || compiler_arg == "como")
      invoke( "Comeau C++ 4.2.44 beta3", "como -o " + program_name 
              + " -I" + path + " " + fullpath, 
              invoke_mode, invoke_args, program_name);

  } else if (platform() == "sunos") {
    if (compiler_arg == "*" || compiler_arg == "gcc")
      invoke("GCC 2.95.2", 
             "g++ " + gcc_flags 
             + " -o " + program_name 
             + " -I"  + path + " " + fullpath, 
             invoke_mode, invoke_args, program_name);
    if (compiler_arg == "*" || compiler_arg == "kcc")
      invoke("KCC 3.4g", "KCC " + kcc_flags
             + " -o " + program_name
             + " -I" + path + " " + fullpath, 
             invoke_mode, invoke_args, program_name);
  } else if (platform() == "beos") {

    if (compiler_arg=="*" || compiler_arg=="gcc")
      invoke( "GNU GCC", "c++ " + gcc_flags 
              +" -o " + program_name
              + " -I" + path + "  " + fullpath, 
              invoke_mode, invoke_args, program_name );

    // shouldn't this next one be called gcc-stlport instead of gcc-sgi?
    if (compiler_arg=="*" || compiler_arg=="gcc-sgi")
      invoke("GNU GCC", "c++ " + gcc_flags 
             + " -o " + program_name
             + " -I/boot/home/config/stlport/stl330 -I" + path
             + "  " + fullpath, 
             invoke_mode, invoke_args, program_name );

  } else {
    if (compiler_arg=="*" || compiler_arg=="bcc") {
      char* path_ptr = getenv("BOOST_BCC55_PATH");
      if (path_ptr == 0) {
	std::cerr << "Environment variable BOOST_BCC55_PATH not defined"
		  << std::endl;
	exit(1);
      }
      std::string bcc55_path = path_ptr;
      invoke( "Borland C++ 5.5.1", "\"" + bcc55_path
              + "/bcc32\" -e" + program_name
              + " -I" + path + " -j10 -q -Ve" + fullpath, 
              invoke_mode, invoke_args, program_name );
    }
    if (compiler_arg=="gcc") {
      // TODO: fix the absolute STLport paths
      invoke( "GNU GCC", "c++ " + gcc_flags 
              + " -o " + program_name
              + " -I" + path + " -IC:/stl/STLport-4.0b8/stlport  " 
              + fullpath
              + "  c:/stl/STLport-4.0b8/lib/libstlport_gcc.a", 
              invoke_mode, invoke_args, program_name );
    }
    if (compiler_arg=="*" || compiler_arg=="cw")
      invoke( "Metrowerks CodeWarrior", 
              "mwcc " + mwcc_flags 
              + " -I- -o " + program_name
              + " -I" + path + " " + fullpath, 
              invoke_mode, invoke_args, program_name );

    if (compiler_arg=="*" || compiler_arg=="vc")
      invoke( "VC++ with MS library",
              "cl  -o " + program_name 
              + " " + msvc_flags
              + " /I " + path + " " + fullpath + " user32.lib", 
              invoke_mode, invoke_args, program_name );

    if (compiler_arg=="*" || compiler_arg=="vcstlport") {
      char* path_ptr = getenv("BOOST_STLPORT_PATH");
      if (path_ptr == 0) {
	std::cerr << "Environment variable BOOST_STLPORT_PATH not defined"
		  << std::endl;
	exit(1);
      }
      std::string stlport = path_ptr;
      invoke( "VC++ with STLport library", 
              "cl -o " + program_name + msvc_flags 
              + "/I " + stlport + " /I " + path + fullpath + " user32.lib", 
              invoke_mode, invoke_args, program_name );
    }
  }
  
  outfile << "</tr>" << std::endl;
}

//-----------------------------------------------------------------------------

void library()
{
  std::cout << std::endl
            << "***** Boost Library *****" << std::endl;

  outfile << "<tr>" << std::endl
          << "<td>Boost library build</td>" << std::endl;

  // ...

  outfile << "</tr>" << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  char* path_ptr = getenv("BOOST_PATH");
  if (path_ptr == 0) {
    std::cerr << "Environment variable BOOST_PATH not defined" << std::endl;
    return -1;
  }
  path = path_ptr;

  compiler_arg = "*";
  if (argc > 1)
    compiler_arg = argv[1];

  program_arg = "*";
  if (argc > 2)
    program_arg = argv[2];
  
  if (platform() == "unkown") {
    std::cerr << "**** Error: unknown platform ****" << std::endl;
    return 1;
  }

  if (platform() == "windows") {
    exe_suffix = ".exe";
    exe_invoke_prefix = "";
  } else {
    exe_suffix = "";
    exe_invoke_prefix = "./";
  }

  std::string filename = "cs-" + platform() + ".htm";
  outfile.open(filename.c_str());

  time_t today;
  time(&today);

  outfile << "<html>\n<head>\n<title>\nCompiler Status: " << platform() << "\n</title>\n</head>" 
          << "<body bgcolor=\"#FFFFFF\" text=\"#000000\">" << std::endl 
          << "<h1><img border=\"0\" src=\"../c++boost.gif\" width=\"277\" height=\"86\"></h1>" << std::endl 
          << "<h1>Compiler Status: " << platform() << "</h1>" << std::endl 
          << "<p><b>Run Date:</b> " << ctime(&today)
          << "</p>" << std::endl 
          << "<p>" << std::endl 
          << "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">" << std::endl  
          << "<tr>" << std::endl 
          << "<td>Program</td>" << std::endl;
  
  if (platform() == "linux") { 
    if (compiler_arg == "*" || compiler_arg == "gcc")
      outfile << "<td>GNU<br>GCC<br>2.95.2</td>" << std::endl;
    if (compiler_arg == "*" || compiler_arg == "gcc-stlport")
      outfile << "<td>GNU<br>GCC<br>2.95.2<br>STLport<br>4.0</td>" 
              << std::endl;
#if 0
  if (compiler_arg == "*" || compiler_arg == "gcc-exp")
    outfile << "<td>GNU<br>GCC<br>pre-2.97 experimental</td>" << std::endl;
#endif
  if (compiler_arg == "*" || compiler_arg == "como")
    outfile << "<td>Comeau C++<br>4.2.44 beta3<br>STLport<br>4.0</td>" 
            << std::endl;
#if 0
  if (compiler_arg == "*" || compiler_arg == "occ")
    outfile << "<td>OpenC++<br>2.5.9</td>" << std::endl;
#endif
  } else if (platform() == "sunos") {
    if (compiler_arg == "*" || compiler_arg == "suncc")
      outfile << "<td>Sun C++<br>Sun WorkShop 6, C++ 5.1</td>" << std::endl;
    if (compiler_arg == "*" || compiler_arg == "gcc")
      outfile << "<td>GNU<br>GCC<br>2.95.2</td>" << std::endl;
    if (compiler_arg == "*" || compiler_arg == "kcc")
      outfile << "<td>KAI<br>KCC<br>3.4g</td>" << std::endl;
  } else if (platform() == "beos") {
    if (compiler_arg == "*" || compiler_arg == "gcc")
      outfile << "<td>GNUPro<br>GCC&nbsp;2.9</td>" << std::endl;
    if (compiler_arg == "*" || compiler_arg == "gcc-sgi")
      outfile
        << "<td>GNUPro<br>GCC&nbsp;2.9<br>+<br>SGI&nbsp;STL&nbsp;3.3</td>" 
        << std::endl;
  } else {
#if 0
    if (compiler_arg=="*" || compiler_arg=="bcc54")
      outfile << "<td>Borland<br>BCC<br>5.4 up2</td>" << std::endl;
#endif
    if (compiler_arg=="*" || compiler_arg=="bcc")
      outfile << "<td>Borland<br>BCC<br>5.5.1</td>" << std::endl;
    
    // GCC 2.95.2 is looping on some tests, so only invoke if asked
    // for by name
    if (compiler_arg=="gcc")
      outfile << "<td>GNU<br>GCC<br>2.95.2<br>STLport<br>4.0 beta 8</td>" 
              << std::endl;
    if (compiler_arg=="*" || compiler_arg=="cw")
      outfile << "<td>Metrowerks<br>CodeWarrior<br>6.0</td>" << std::endl;
    if (compiler_arg=="*" || compiler_arg=="vc")
      outfile << "<td>Microsoft<br>VC++<br>6.0 SP4</td>" << std::endl;
    if (compiler_arg=="*" || compiler_arg=="vcstlport")
      outfile << "<td>Microsoft<br>VC++<br>6.0 SP4<br>STLport<br>4.0</td>"
              << std::endl;
  }

  outfile << "</tr>" << std::endl;

  if  (program_arg == "*") {
    std::string filelist = "regrtest_files.txt";
    std::ifstream regr_files(filelist.c_str());
    if (regr_files) {
      const int max_line = 1000;
      char line[max_line];
      while (regr_files.getline(line, max_line)) {
        char *program_ptr, *mode_ptr, *arg_ptr;
	program_ptr = strtok(line, " ");
	if (program_ptr == 0) {
	  std::cerr << "file format error, no program file name" << std::endl;
	  return -1;
	}
	mode_ptr = strtok(0, " \n");
	if (mode_ptr == 0) {
	  std::cerr << "file format error, no mode character" << std::endl;
	  return -1;
	}
	arg_ptr = strtok(0, " \n");
	char* empty_string = "";
	if (arg_ptr == 0)
	  arg_ptr = empty_string;

        compile(program_ptr, *mode_ptr, arg_ptr, "regress" + exe_suffix);
      }
    } else {
      std::cerr << "Could not open regression test file list: " 
                << filelist << std::endl;
      return -1;
    }
  } else
    compile(program_arg, 'C', "", "regress" + exe_suffix);

  outfile << "</table>" << std::endl;

  if (platform() == "linux")
    outfile << "<p>\nNote: A hand-crafted &lt;limits&gt; " 
            << "Standard header has been applied to all configurations." 
            << std::endl;

  outfile << "</body>\n</html>" << std::endl;

  return 0;
}
