//  Generate Compiler Status HTML from jam regression test output  -----------//

//  (C) Copyright Beman Dawes 2002. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

/*******************************************************************************

    This program was designed to work unchanged on all platforms and
    configurations.  All output which is platform or configuration dependent
    is obtained from external sources such as the .xml file from
    process_jam_log execution, the tools/build/xxx-tools.jam files, or the
    output of the config_info tests.

    Please avoid adding platform or configuration dependencies during
    program maintenance.

*******************************************************************************/

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/fstream.hpp"
#include "detail/tiny_xml.hpp"
namespace fs = boost::filesystem;
namespace xml = boost::tiny_xml;

#include <cstdlib>  // for abort
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdexcept>

using std::string;

const string pass_msg( "Pass" );
const string warn_msg( "<font color=\"#FF9900\"><i>Warn</i></font>" );
const string fail_msg( "<font color=\"#FF0000\">"
                      "<big><i>Fail</i></big>"
                       "</font>" );
const string missing_residue_msg( "<i>Missing</i>" );

const std::size_t max_compile_msg_size = 10000;

namespace
{
  fs::path boost_root;  // boost-root complete path
  fs::path locate_root; // locate-root (AKA ALL_LOCATE_TARGET) complete path

  bool ignore_pass;
  bool no_warn;
  bool no_links;

  fs::directory_iterator end_itr;

  // It's immportant for reliability that we find the same compilers for each
  // test, and that they match the column header.  So save the names at the
  // time column headings are generated.
  std::vector<string> toolsets;

  fs::ifstream jamfile;
  fs::ofstream report;
  fs::ofstream links_file;
  string links_name;

  string specific_compiler; // if running on one toolset only

  const string empty_string;

//  relative path between two paths  -----------------------------------------//

  void relative_path( const fs::path & from, const fs::path & to,
    fs::path & target )
  {
    if ( from.string().size() <= to.string().size() ) return;
    target /= "..";
    relative_path( from.branch_path(), to, target );
    return;
  }

//  extract test name from target directory string  --------------------------//

  string extract_test_name( const string & s )
  {
    string t( s );
    string::size_type pos = t.find( "/bin/" );
    if ( pos != string::npos ) pos += 5;
    else return "";
    return t.substr( pos, t.find( ".", pos ) - pos );
  }

//  find_file  ---------------------------------------------------------------//
//  given a directory to recursively search

  bool find_file( const fs::path & dir_path, const string & name,
    fs::path & path_found, const string & ignore_dir_named="" )
  {
    if ( !fs::exists( dir_path ) ) return false;
    for ( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
      if ( fs::is_directory( *itr )
        && itr->leaf() != ignore_dir_named )
      {
        if ( find_file( *itr, name, path_found ) ) return true;
      }
      else if ( itr->leaf() == name )
      {
        path_found = *itr;
        return true;
      }
    return false;
  }

//  platform_desc  -----------------------------------------------------------//
//  from locate_root/status/bin/config_info.test/xxx/.../config_info.output

  string platform_desc()
  {
    string result;
    fs::path dot_output_path;

    // the gcc config_info "Detected Platform" sometimes reports "cygwin", so
    // prefer any of the other compilers.
    if ( find_file( locate_root / "status/bin/config_info.test",
      "config_info.output", dot_output_path, "gcc" )
      || find_file( locate_root / "status/bin/config_info.test",
      "config_info.output", dot_output_path ) )
    {
      fs::ifstream file( dot_output_path );
      if ( file )
      {
        while( std::getline( file, result ) )
        {
          if ( result.find( "Detected Platform: " ) == 0 )
          {
            result.erase( 0, 19 );
            return result;
          }
        }
        result.clear();
      }
    }
    return result;
  }

//  version_desc  ------------------------------------------------------------//
//  from locate-root/status/bin/config_info.test/xxx/.../config_info.output

  string version_desc( const string & compiler_name )
  {
    string result;
    fs::path dot_output_path;
    if ( find_file( locate_root / "status/bin/config_info.test"
      / compiler_name, "config_info.output", dot_output_path ) )
    {
      fs::ifstream file( dot_output_path );
      if ( file )
      {
        if( std::getline( file, result ) )
        {
          string::size_type pos = result.find( "version " );
          if ( pos != string::npos )
          {
            result.erase( 0, pos+8 );
          }
          else result.clear();
        }
      }
    }
    return result;
  }

//  compiler_desc  -----------------------------------------------------------//
//  from boost-root/tools/build/xxx-tools.jam

  string compiler_desc( const string & compiler_name )
  {
    string result;
    fs::path tools_path( boost_root / "tools/build" / (compiler_name
      + "-tools.jam") );
    fs::ifstream file( tools_path );
    if ( file )
    {
      while( std::getline( file, result ) )
      {
        if ( result.substr( 0, 3 ) == "#//" )
        {
          result.erase( 0, 3 );
          return result;
        }
      }
      result.clear();
    }
    return result;
  }

//  target_directory  --------------------------------------------------------//
//  this amounts to a request to find a unique leaf directory

  fs::path target_directory( const fs::path & root )
  {
    if ( !fs::exists( root ) ) return fs::path("no-such-path");
    fs::path child;
    for ( fs::directory_iterator itr( root ); itr != end_itr; ++itr )
    {
      if ( fs::is_directory( *itr ) )
      {
        if ( child.empty() ) child = *itr;
        else
        {
          std::cout << "Warning: only first of two target possibilities will be reported for: \n "
            << root.string() << ": " << child.leaf()
            << " and " << (*itr).leaf() << "\n";
        }
      }
    }
    if ( child.empty() ) return root; // this dir has no children
    return target_directory( child );
  }

//  element_content  ---------------------------------------------------------//

  const string & element_content(
    const xml::element_ptr & root, const string & name )
  {
    static string empty_string;
    xml::element_list::iterator itr;
    for ( itr = root->elements.begin();
          itr != root->elements.end() && (*itr)->name != name;
          ++itr ) {}
    return itr != root->elements.end() ? (*itr)->content : empty_string;
  }

//  find_attribute  ----------------------------------------------------------//

const string & attribute_value( const xml::element_ptr & element,
                                const string & attribute_name )
{
  static const string empty_string;
  xml::attribute_list::iterator atr;
  for ( atr = element->attributes.begin();
        atr != element->attributes.end() && atr->name != attribute_name;
        ++atr ) {}
  return atr == element->attributes.end() ? empty_string : atr->value;
}

//  generate_report  ---------------------------------------------------------//

  // return 0 if nothing generated, 1 otherwise, except 2 if compiler msgs
  int generate_report( const xml::element_ptr & db,
                       const string & test_name,
                       const string & toolset,
                       bool pass,
                       bool always_show_run_output = false )
  {
    // compile msgs sometimes modified, so make a local copy
    string compile( (pass && no_warn)
      ? empty_string :  element_content( db, "compile" ) );

    const string & link( pass ? empty_string : element_content( db, "link" ) );
    const string & run( (pass && !always_show_run_output)
      ? empty_string : element_content( db, "run" ) );
    string lib( pass ? empty_string : element_content( db, "lib" ) );

    // some compilers output the filename even if there are no errors or
    // warnings; detect this if one line of output and it contains no space.
    string::size_type pos = compile.find( '\n', 1 );
    if ( pos != string::npos && compile.size()-pos <= 2
        && compile.find( ' ' ) == string::npos ) compile.clear();

    if ( lib.empty() && compile.empty() && link.empty() && run.empty() )
      return 0;

    int result = 1; // some kind of msg for sure

    // limit compile message length
    if ( compile.size() > max_compile_msg_size )
    {
      compile.erase( max_compile_msg_size );
      compile += "...\n   (remainder deleted because of excessive size)\n";
    }

    links_file << "<h2><a name=\""
      << test_name << " " << toolset << "\">"
      << test_name << " / " << toolset << "</a></h2>\n";

    if ( !compile.empty() )
    {
      ++result;
      links_file << "<h3>Compiler output:</h3><pre>"
        << compile << "</pre>\n";
    }
    if ( !link.empty() )
      links_file << "<h3>Linker output:</h3><pre>" << link << "</pre>\n";
    if ( !run.empty() )
      links_file << "<h3>Run output:</h3><pre>" << run << "</pre>\n";

    static std::set< string > failed_lib_target_dirs;
    if ( !lib.empty() )
    {
      if ( lib[0] == '\n' ) lib.erase( 0, 1 );
      string lib_test_name( extract_test_name( lib ) );
      links_file << "<h3>Library build failure: </h3>\n"
        "See <a href=\"#" << lib_test_name << " " << toolset << "\">"
        << lib_test_name << " / " << toolset << "</a>";

      if ( failed_lib_target_dirs.find( lib ) == failed_lib_target_dirs.end() )
      {
        failed_lib_target_dirs.insert( lib );
        fs::path pth( boost_root / lib / "test_log.xml" );
        fs::ifstream file( pth );
        if ( file )
        {
          xml::element_ptr db = xml::parse( file, pth.string() );
          generate_report( db, lib_test_name, toolset, false );
        }
        else
        {
          links_file << "<h2><a name=\""
            << lib_test_name << " " << toolset << "\">"
            << lib_test_name << " / " << toolset << "</a></h2>\n"
            "test_log.xml not found\n";
        }
      }
    }
    return result;
  }

  //  do_cell  -----------------------------------------------------------------//

  bool do_cell( const fs::path & test_dir,
    const string & test_name,
    const string & toolset,
    string & target,
    bool always_show_run_output )
  // return true if any results except pass_msg
  {
    fs::path target_dir( target_directory( test_dir / toolset ) );
    bool pass = false;

    // missing jam residue
    if ( fs::exists( target_dir / (test_name + ".test") ) ) pass = true;
    else if ( !fs::exists( target_dir / "test_log.xml" ) )
    {
      target += "<td>" + missing_residue_msg + "</td>";
      return true;
    }

    int anything_generated = 0;

    if ( !no_links )
    {
      fs::path pth( target_dir / "test_log.xml" );
      fs::ifstream file( pth );
      if ( !file ) // missing jam_log.xml
      {
        std::cerr << "Missing jam_log.xml in target:\n "
          << target_dir.string() << "\n";
        target += "<td>";
        target += pass ? pass_msg : fail_msg;
        target += "</td>";
        return pass;
      }
      xml::element_ptr db = xml::parse( file, pth.string() );

      // generate bookmarked report of results, and link to it
      anything_generated
        = generate_report( db, test_name, toolset, pass, always_show_run_output );
    }

    target += "<td>";
    if ( anything_generated != 0 )
    {
      target += "<a href=\"";
      target += links_name;
      target += "#";
      target += test_name;
      target += " ";
      target += toolset;
      target += "\">";
      target += pass ? (anything_generated < 2 ? pass_msg : warn_msg) : fail_msg;
      target += "</a>";
    }
    else  target += pass ? pass_msg : fail_msg;
    target += "</td>";
    return (anything_generated != 0) || !pass;
  }

//  do_row  ------------------------------------------------------------------//

  void do_row(
    const fs::path & test_dir, // locate_root / "status/bin/any_test.test"
    const string & test_name, // "any_test"
    string & target )
  {
    // get library name, test-type, test-program path, etc., from the .xml file
    string lib_name;
    string test_path( test_name ); // test_name is default if missing .test
    string test_type( "unknown" );
    bool always_show_run_output( false );
    fs::path xml_file_path;
    if ( find_file( test_dir, "test_log.xml", xml_file_path ) )
    {
      fs::ifstream file( xml_file_path );
      if ( file )
      {
        xml::element_ptr db = xml::parse( file, xml_file_path.string() );
        test_path = attribute_value( db, "test-program" );
        lib_name = attribute_value( db, "library" );
        test_type = attribute_value( db, "test-type" );
        always_show_run_output
          = attribute_value( db, "show-run-output" ) == "true";
      }
    }

    // path to docs
    fs::path rel;
    relative_path( fs::initial_path(), boost_root, rel );
    string lib_docs_path( rel.string() + "/libs/" + lib_name );

    // generate the library name, test name, and test type table data
    string::size_type row_start_pos = target.size();
    target += "<tr><td><a href=\"" + lib_docs_path + "\">"  + lib_name  + "</a></td>";
    target += "<td><a href=\"" + (rel / test_path).string() + "\">" + test_name + "</a></td>";
    target += "<td>" + test_type + "</td>";

    bool no_warn_save = no_warn;
    if ( test_type.find( "fail" ) != string::npos ) no_warn = true;

    // for each compiler, generate <td>...</td> html
    bool anything_to_report = false;
    for ( std::vector<string>::const_iterator itr=toolsets.begin();
      itr != toolsets.end(); ++itr )
    {
      anything_to_report |= do_cell( test_dir, test_name, *itr, target,
        always_show_run_output );
    }

    target += "</tr>";
    if ( ignore_pass && !anything_to_report ) target.erase( row_start_pos );
    no_warn = no_warn_save;
  }

//  do_rows_for_sub_tree  ----------------------------------------------------//

  void do_rows_for_sub_tree(
    const fs::path & bin_dir, std::vector<string> & results )
  {
    for ( fs::directory_iterator itr( bin_dir ); itr != end_itr; ++itr )
    {
      if ( fs::is_directory( *itr )
        && itr->string().find( ".test" ) == (itr->string().size()-5) )
      {
        results.push_back( std::string() ); 
        do_row( *itr,
                itr->leaf().substr( 0, itr->leaf().size()-5 ),
                results[results.size()-1] );
      }
    }
  }

//  do_table_body  -----------------------------------------------------------//

  void do_table_body( const fs::path & bin_dir )
  {
    // rows are held in a vector so they can be sorted, if desired.
    std::vector<string> results;

    // do primary bin directory
    do_rows_for_sub_tree( bin_dir, results );

    // do subinclude bin directories
    jamfile.clear();
    jamfile.seekg(0);
    string line;
    while( std::getline( jamfile, line ) )
    {
      string::size_type pos( line.find( "subinclude" ) );
      if ( pos != string::npos
        && line.find( '#' ) > pos )
      {
        pos = line.find_first_not_of( " \t", pos+10 );
        if ( pos == string::npos ) continue;
        string subinclude_bin_dir(
          line.substr( pos, line.find_first_of( " \t", pos )-pos ) );
//      std::cout << "subinclude: " << subinclude_bin_dir << '\n';
        fs::path subinclude_path( locate_root / subinclude_bin_dir / "/bin" );
        if ( fs::exists( subinclude_path ) )
          { do_rows_for_sub_tree( subinclude_path, results ); }
      }
    }


    std::sort( results.begin(), results.end() );

    for ( std::vector<string>::iterator v(results.begin());
      v != results.end(); ++v )
      { report << *v << "\n"; }
  }

//  do_table  ----------------------------------------------------------------//

  void do_table()
  {
    string relative( fs::initial_path().string() );
    relative.erase( 0, boost_root.string().size()+1 );
    fs::path bin_path( locate_root / relative / "bin" );

    report << "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">\n";

    // generate the column headings

    report << "<tr><td>Library</td><td>Test Name</td>\n"
      "<td><a href=\"compiler_status.html#test-type\">Test Type</a></td>\n";

    fs::directory_iterator itr( bin_path );
    while ( itr != end_itr && !fs::is_directory( *itr )
      && itr->string().find( ".test" ) != (itr->string().size()-5) )
      ++itr; // bypass chaff
    if ( itr != end_itr )
    {
      fs::directory_iterator compiler_itr( *itr );
      if ( specific_compiler.empty() )
        std::clog << "Using " << itr->string() << " to determine compilers\n";
      for (; compiler_itr != end_itr; ++compiler_itr )
      {
        if ( fs::is_directory( *compiler_itr )  // check just to be sure
          && compiler_itr->leaf() != "test" ) // avoid strange directory (Jamfile bug?)
        {
          if ( specific_compiler.size() != 0
            && specific_compiler != compiler_itr->leaf() ) continue;
          toolsets.push_back( compiler_itr->leaf() );
          string desc( compiler_desc( compiler_itr->leaf() ) );
          string vers( version_desc( compiler_itr->leaf() ) );
          report << "<td>"
               << (desc.size() ? desc : compiler_itr->leaf())
               << (vers.size() ? (string( "<br>" ) + vers ) : string( "" ))
               << "</td>\n";
        }
      }
    }

    report << "</tr>\n";

    // now the rest of the table body

    do_table_body( bin_path );

    report << "</table>\n";
  }

} // unnamed namespace

//  main  --------------------------------------------------------------------//

#define BOOST_NO_CPP_MAIN_SUCCESS_MESSAGE
#include <boost/test/included/prg_exec_monitor.hpp>

int cpp_main( int argc, char * argv[] ) // note name!
{
  while ( argc > 1 && *argv[1] == '-' )
  {
    if ( argc > 2 && std::strcmp( argv[1], "--compiler" ) == 0 )
    { specific_compiler = argv[2]; --argc; ++argv; }
    else if ( argc > 2 && std::strcmp( argv[1], "--locate-root" ) == 0 )
    { locate_root = fs::path( argv[2], fs::native ); --argc; ++argv; }
    else if ( std::strcmp( argv[1], "--ignore-pass" ) == 0 ) ignore_pass = true;
    else if ( std::strcmp( argv[1], "--no-warn" ) == 0 ) no_warn = true;
    else { std::cerr << "Unknown option: " << argv[1] << "\n"; argc = 1; }
    --argc;
    ++argv;
  }

  if ( argc != 3 && argc != 4 )
  {
    std::cerr <<
      "Usage: compiler_status [options...] boost-root status-file [links-file]\n"
      "  boost-root is the path to the boost tree root directory.\n"
      "  status-file and links-file are paths to the output files.\n"
      "Must be run from directory containing Jamfile\n"
      "  options: --compiler name     Run for named compiler only\n"
      "           --ignore-pass       Do not report tests which pass all compilers\n"
      "           --no-warn           Warnings not reported if test passes\n"
      "           --locate-root path  Path to ALL_LOCATE_TARGET for bjam;\n"
      "                               default boost-root.\n"
      "example: compiler_status --compiler gcc /boost-root cs.html cs-links.html\n";
    return 1;
  }

  boost_root = fs::path( argv[1], fs::native );
  if ( locate_root.empty() ) locate_root = boost_root;

  fs::path jamfile_path( fs::initial_path() / "Jamfile" );
  jamfile.open( jamfile_path );
  if ( !jamfile )
  {
    std::cerr << "Could not open Jamfile: " << jamfile_path.native_file_string() << std::endl;
    return 1;
  }

  report.open( fs::path( argv[2], fs::native ) );
  if ( !report )
  {
    std::cerr << "Could not open report output file: " << argv[2] << std::endl;
    return 1;
  }

  if ( argc == 4 )
  {
    fs::path links_path( argv[3], fs::native );
    links_name = links_path.leaf();
    links_file.open( links_path );
    if ( !links_file )
    {
      std::cerr << "Could not open links output file: " << argv[3] << std::endl;
      return 1;
    }
  }
  else no_links = true;

  char run_date[128];
  std::time_t tod;
  std::time( &tod );
  std::strftime( run_date, sizeof(run_date),
    "%X UTC, %A %d %B %Y", std::gmtime( &tod ) );

  report << "<html>\n"
          "<head>\n"
          "<title>Boost Compiler Status Automatic Test</title>\n"
          "</head>\n"
          "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
          "<table border=\"0\">\n"
          "<tr>\n"
          "<td><img border=\"0\" src=\"../c++boost.gif\" width=\"277\" "
          "height=\"86\"></td>\n"
          "<td>\n"
          "<h1>Compiler Status: " + platform_desc() + "</h1>\n"
          "<b>Run Date:</b> "
       << run_date
       << "\n</td>\n</table>\n<br>\n"
       ;

  if ( !no_links )
  {
    links_file
      << "<html>\n"
         "<head>\n"
         "<title>Boost Compiler Status Error Log</title>\n"
         "</head>\n"
         "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
         "<table border=\"0\">\n"
         "<tr>\n"
         "<td><img border=\"0\" src=\"../c++boost.gif\" width=\"277\" "
         "height=\"86\"></td>\n"
         "<td>\n"
         "<h1>Compiler Status: " + platform_desc() + "</h1>\n"
         "<b>Run Date:</b> "
      << run_date
      << "\n</td>\n</table>\n<br>\n"
      ;
  }

  do_table();

  report << "</body>\n"
          "</html>\n"
          ;

  if ( !no_links )
  {
    links_file
      << "</body>\n"
         "</html>\n"
         ;
  }
  return 0;
}
