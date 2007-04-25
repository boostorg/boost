//  Generate Compiler Status HTML from jam regression test output  -----------//

//  Copyright Beman Dawes 2002.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/tools/regression/ for documentation.

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

#include <cstdlib>  // for abort, exit
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <cassert>

using std::string;

const string pass_msg( "Pass" );
const string warn_msg( "<i>Warn</i>" );
const string fail_msg( "<font color=\"#FF0000\"><i>Fail</i></font>" );
const string note_msg( "<sup>*</sup>" );
const string missing_residue_msg( "<i>Missing</i>" );

const std::size_t max_compile_msg_size = 10000;

namespace
{
  fs::path boost_root;  // boost-root complete path
  fs::path locate_root; // locate-root (AKA ALL_LOCATE_TARGET) complete path

  bool ignore_pass;
  bool no_warn;
  bool no_links;
  bool boost_build_v2;

  fs::path jamfile_path;

  fs::directory_iterator end_itr;

  // It's immportant for reliability that we find the same compilers for each
  // test, and that they match the column header.  So save the names at the
  // time column headings are generated.
  std::vector<string> toolsets;

  fs::ifstream jamfile;
  fs::ofstream report;
  fs::ofstream links_file;
  string links_name;

  fs::path notes_path;
  string notes_html;

  fs::path notes_map_path;
  typedef std::multimap< string, string > notes_map; // key is test_name-toolset,
                                                // value is note bookmark
  notes_map notes;

  string specific_compiler; // if running on one toolset only

  const string empty_string;

  // prefix for library and test hyperlink prefix
  string cvs_root ( "http://boost.cvs.sourceforge.net/" );
  string url_prefix_dir_view( cvs_root + "boost/boost" );
  string url_prefix_checkout_view( cvs_root + "*checkout*/boost/boost" );
  string url_suffix_text_view( "?view=markup&rev=HEAD" );

//  build notes_bookmarks from notes HTML  -----------------------------------//

  void build_notes_bookmarks()
  {
    if ( notes_map_path.empty() ) return;
    fs::ifstream notes_map_file( notes_map_path );
    if ( !notes_map_file )
    {
      std::cerr << "Could not open --notes-map input file: " << notes_map_path.string() << std::endl;
      std::exit( 1 );
    }
    string line;
    while( std::getline( notes_map_file, line ) )
    {
      string::size_type pos = 0;
      if ( (pos = line.find( ',', pos )) == string::npos ) continue;
      string key(line.substr( 0, pos ) );
      string bookmark( line.substr( pos+1 ) );

//      std::cout << "inserting \"" << key << "\",\"" << bookmark << "\"\n";
      notes.insert( notes_map::value_type( key, bookmark ) );
    }
  }

//  load_notes_html  ---------------------------------------------------------//

  bool load_notes_html()
  {
    if ( notes_path.empty() ) return false;
    fs::ifstream notes_file( notes_path );
    if ( !notes_file )
    {
      std::cerr << "Could not open --notes input file: " << notes_path.string() << std::endl;
      std::exit( 1 );
    }
    string line;
    bool in_body( false );
    while( std::getline( notes_file, line ) )
    {
      if ( in_body && line.find( "</body>" ) != string::npos ) in_body = false;
      if ( in_body ) notes_html += line;
      else if ( line.find( "<body>" ) ) in_body = true;
    }
    return true;
  }

//  relative path between two paths  -----------------------------------------//

  void relative_path( const fs::path & from, const fs::path & to,
    fs::path & target )
  {
    if ( from.string().size() <= to.string().size() ) return;
    target /= "..";
    relative_path( from.branch_path(), to, target );
    return;
  }

//  extract object library name from target directory string  ----------------//

  string extract_object_library_name( const string & s )
  {
    string t( s );
    string::size_type pos = t.find( "/build/" );
    if ( pos != string::npos ) pos += 7;
    else if ( (pos = t.find( "/test/" )) != string::npos ) pos += 6;
    else return "";
    return t.substr( pos, t.find( "/", pos ) - pos );
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
    if ( find_file( locate_root / "bin/boost/status/config_info.test",
      "config_info.output", dot_output_path, "gcc" )
      || find_file( locate_root / "bin/boost/status/config_info.test",
      "config_info.output", dot_output_path )
      || find_file( locate_root / "status/bin/config_info.test",
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
    if ( find_file( locate_root / "bin/boost/status/config_info.test"
      / compiler_name, "config_info.output", dot_output_path )
      || find_file( locate_root / "status/bin/config_info.test"
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
    fs::path tools_path( boost_root / "tools/build/v1" / (compiler_name
      + "-tools.jam") );
    if ( !fs::exists( tools_path ) )
      tools_path = boost_root / "tools/build" / (compiler_name + "-tools.jam");
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
        // SunCC creates an internal subdirectory everywhere it writes
        // object files.  This confuses the target_directory() algorithm.
        // This patch ignores the SunCC internal directory. Jens Maurer
        if ( (*itr).leaf() == "SunWS_cache" ) continue;
        // SGI does something similar for template instantiations. Jens Maurer
        if(  (*itr).leaf() == "ii_files" ) continue; 

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
    const xml::element & root, const string & name )
  {
    static string empty_string;
    xml::element_list::const_iterator itr;
    for ( itr = root.elements.begin();
          itr != root.elements.end() && (*itr)->name != name;
          ++itr ) {}
    return itr != root.elements.end() ? (*itr)->content : empty_string;
  }

//  find_element  ------------------------------------------------------------//

  const xml::element empty_element;

  const xml::element & find_element(
    const xml::element & root, const string & name )
  {
    xml::element_list::const_iterator itr;
    for ( itr = root.elements.begin();
          itr != root.elements.end() && (*itr)->name != name;
          ++itr ) {}
    return itr != root.elements.end() ? *((*itr).get()) : empty_element;
  }

//  attribute_value  ----------------------------------------------------------//

const string & attribute_value( const xml::element & element,
                                const string & attribute_name )
{
  static const string empty_string;
  xml::attribute_list::const_iterator atr;
  for ( atr = element.attributes.begin();
        atr != element.attributes.end() && atr->name != attribute_name;
        ++atr ) {}
  return atr == element.attributes.end() ? empty_string : atr->value;
}

//  find_bin_path  -----------------------------------------------------------//

// Takes a relative path from boost root to a Jamfile.
// Returns the directory where the build targets from
// that Jamfile are located. If not found, emits a warning 
// and returns empty path.
const fs::path find_bin_path(const string& relative)
{
  fs::path bin_path;
  if (boost_build_v2)
  {
    bin_path = locate_root / "bin.v2" / relative;
    if (!fs::exists(bin_path))
    {
      std::cerr << "warning: could not find build results for '" 
                << relative << "'.\n";
      std::cerr << "warning: tried directory " 
                << bin_path.native_directory_string() << "\n";
      bin_path = "";
    }
  }
  else
  {
    bin_path = locate_root / "bin/boost" / relative;
    if (!fs::exists(bin_path))
    {
      bin_path = locate_root / "bin" / relative / "bin";
      if (!fs::exists(bin_path))
      {
        bin_path = fs::path( locate_root / relative / "bin" );
        if (!fs::exists(bin_path))
        {
          bin_path = fs::path( locate_root / "bin/boost/libs" /
            relative.substr( relative.find( '/' )+1 ) );
        }
      }
    }
    if (!fs::exists(bin_path))
    {
      std::cerr << "warning: could not find build results for '" 
                << relative << "'.\n";
      bin_path = "";
    }
  }
  return bin_path;
}


//  generate_report  ---------------------------------------------------------//

  // return 0 if nothing generated, 1 otherwise, except 2 if compiler msgs
  int generate_report( const xml::element & db,
                       const string & source_library_name,
                       const string & test_type,
                       const string & test_name, // possibly object library name
                       const string & toolset,
                       bool pass,
                       bool always_show_run_output = false )
  {
    // compile msgs sometimes modified, so make a local copy
    string compile( ((pass && no_warn)
      ? empty_string :  element_content( db, "compile" )) );

    const string & link( pass ? empty_string : element_content( db, "link" ) );
    const string & run( (pass && !always_show_run_output)
      ? empty_string : element_content( db, "run" ) );
    string lib( (pass ? empty_string : element_content( db, "lib" )) );

    // some compilers output the filename even if there are no errors or
    // warnings; detect this if one line of output and it contains no space.
    string::size_type pos = compile.find( '\n', 1 );
    if ( pos != string::npos && compile.size()-pos <= 2
        && compile.find( ' ' ) == string::npos ) compile.clear();

    if ( lib.empty()
      && (compile.empty() || test_type == "compile_fail")
      && link.empty() && run.empty() ) return 0;

    int result = 1; // some kind of msg for sure

    // limit compile message length
    if ( compile.size() > max_compile_msg_size )
    {
      compile.erase( max_compile_msg_size );
      compile += "...\n   (remainder deleted because of excessive size)\n";
    }

    links_file << "<h2><a name=\""
      << source_library_name << "-" << test_name << "-" << toolset << "\">"
      << source_library_name << " - " << test_name << " - " << toolset << "</a></h2>\n";

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

    // for an object library failure, generate a reference to the object
    // library failure message, and (once only) generate the object
    // library failure message itself
    static std::set< string > failed_lib_target_dirs; // only generate once
    if ( !lib.empty() )
    {
      if ( lib[0] == '\n' ) lib.erase( 0, 1 );
      string object_library_name( extract_object_library_name( lib ) );

      // changing the target directory naming scheme breaks
      // extract_object_library_name()
      assert( !object_library_name.empty() );
      if ( object_library_name.empty() )
        std::cerr << "Failed to extract object library name from " << lib << "\n";

      links_file << "<h3>Library build failure: </h3>\n"
        "See <a href=\"#"
        << source_library_name << "-"
        << object_library_name << "-" << toolset << "\">"
        << source_library_name << " - "
        << object_library_name << " - " << toolset << "</a>";

      if ( failed_lib_target_dirs.find( lib ) == failed_lib_target_dirs.end() )
      {
        failed_lib_target_dirs.insert( lib );
        fs::path pth( locate_root / lib / "test_log.xml" );
        fs::ifstream file( pth );
        if ( file )
        {
          xml::element_ptr db = xml::parse( file, pth.string() );
          generate_report( *db, source_library_name, test_type, object_library_name, toolset, false );
        }
        else
        {
          links_file << "<h2><a name=\""
            << object_library_name << "-" << toolset << "\">"
            << object_library_name << " - " << toolset << "</a></h2>\n"
            "test_log.xml not found\n";
        }
      }
    }
    return result;
  }

  //  add_notes --------------------------------------------------------------//

  void add_notes( const string & key, bool fail, string & sep, string & target )
  {
    notes_map::const_iterator itr = notes.lower_bound( key );
    if ( itr != notes.end() && itr->first == key )
    {
      for ( ; itr != notes.end() && itr->first == key; ++itr )
      {
        string note_desc( itr->second[0] == '-'
          ? itr->second.substr( 1 ) : itr->second );
        if ( fail || itr->second[0] == '-' )
        {
          target += sep;
          sep = ",";
          target += "<a href=\"";
          target += "#";
          target += note_desc;
          target += "\">";
          target += note_desc;
          target += "</a>";
        }
      }
    }
  }

  //  get_notes  -------------------------------------------------------------//

  string get_notes( const string & toolset,
                    const string & library, const string & test, bool fail )
  {
    string sep;
    string target( "<sup>" );
    add_notes( toolset + "/" + library + "/" + test, fail, sep, target ); 
    add_notes( "*/" + library + "/" + test, fail, sep, target ); 
    add_notes( toolset + "/" + library + "/*", fail, sep, target ); 
    add_notes( "*/" + library + "/*", fail, sep, target );
    if ( target == "<sup>" ) target.clear();
    else target += "</sup>";
    return target;
  }

  //  do_cell  ---------------------------------------------------------------//

  bool do_cell( const string & lib_name,
    const fs::path & test_dir,
    const string & test_type,
    const string & test_name,
    const string & toolset,
    string & target,
    bool always_show_run_output )
  // return true if any results except pass_msg
  {
    fs::path target_dir( target_directory( test_dir / toolset ) );
    bool pass = false;

    if ( !fs::exists( target_dir / "test_log.xml" ) )
    {
      std::cerr << "Missing jam_log.xml in target:\n "
        << target_dir.string() << "\n";
      target += "<td>" + missing_residue_msg + "</td>";
      return true;
    }

    int anything_generated = 0;
    bool note = false;

    fs::path pth( target_dir / "test_log.xml" );
    fs::ifstream file( pth );
    if ( !file ) // could not open jam_log.xml
    {
      std::cerr << "Can't open jam_log.xml in target:\n "
        << target_dir.string() << "\n";
      target += "<td>" + missing_residue_msg + "</td>";
      return false;
    }

    xml::element_ptr dbp = xml::parse( file, pth.string() );
    const xml::element & db( *dbp );

    std::string test_type_base( test_type );
    if ( test_type_base.size() > 5 )
    {
      const string::size_type trailer = test_type_base.size() - 5;
      if ( test_type_base.substr( trailer ) == "_fail" )
      {
        test_type_base.erase( trailer );
      }
    }
    const xml::element & test_type_element( find_element( db, test_type_base ) );

    pass = !test_type_element.name.empty()
      && attribute_value( test_type_element, "result" ) != "fail";

    if ( !no_links )
    {
      note = attribute_value( test_type_element, "result" ) == "note";

      // generate bookmarked report of results, and link to it
      anything_generated
        = generate_report( db, lib_name, test_type, test_name, toolset, pass,
          always_show_run_output || note );
    }

    // generate the status table cell pass/warn/fail HTML
    target += "<td>";
    if ( anything_generated != 0 )
    {
      target += "<a href=\"";
      target += links_name;
      target += "#";
      target += lib_name;
      target += "-";
      target += test_name;
      target += "-";
      target += toolset;
      target += "\">";
      target += pass
        ? (anything_generated < 2 ? pass_msg : warn_msg)
        : fail_msg;
      target += "</a>";
      if ( pass && note ) target += note_msg;
    }
    else  target += pass ? pass_msg : fail_msg;

    // if notes, generate the superscript HTML
    if ( !notes.empty() ) 
      target += get_notes( toolset, lib_name, test_name, !pass );

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
        xml::element_ptr dbp = xml::parse( file, xml_file_path.string() );
        const xml::element & db( *dbp );
        test_path = attribute_value( db, "test-program" );
        lib_name = attribute_value( db, "library" );
        test_type = attribute_value( db, "test-type" );
        always_show_run_output
          = attribute_value( db, "show-run-output" ) == "true";
      }
    }

    // generate the library name, test name, and test type table data
    string::size_type row_start_pos = target.size();
    target += "<tr><td><a href=\"" + url_prefix_dir_view + "/libs/" + lib_name
      + "\">"  + lib_name  + "</a></td>";
    target += "<td><a href=\"" + url_prefix_checkout_view + "/" + test_path
      + url_suffix_text_view + "\">" + test_name + "</a></td>";
    target += "<td>" + test_type + "</td>";

    bool no_warn_save = no_warn;
    //if ( test_type.find( "fail" ) != string::npos ) no_warn = true;

    // for each compiler, generate <td>...</td> html
    bool anything_to_report = false;
    for ( std::vector<string>::const_iterator itr=toolsets.begin();
      itr != toolsets.end(); ++itr )
    {
      anything_to_report |= do_cell( lib_name, test_dir, test_type, test_name, *itr, target,
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
      bool v2(false);
      string::size_type pos( line.find( "subinclude" ) );
      if ( pos == string::npos ) {
        pos = line.find( "build-project" );
        v2 = true;
      }
      if ( pos != string::npos
        && line.find( '#' ) > pos )
      {
        if (v2)
          pos = line.find_first_not_of( " \t./", pos+13 );
        else
          pos = line.find_first_not_of( " \t./", pos+10 );
      
        if ( pos == string::npos ) continue;
        string subinclude_bin_dir(
          line.substr( pos, line.find_first_of( " \t", pos )-pos ) );

        fs::path bin_path = find_bin_path(subinclude_bin_dir);
        if (!bin_path.empty())
          do_rows_for_sub_tree( bin_path, results );
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
    // Find test result locations, trying:
    // - Boost.Build V1 location with ALL_LOCATE_TARGET
    // - Boost.Build V2 location with top-lelve "build-dir" 
    // - Boost.Build V1 location without ALL_LOCATE_TARGET
    string relative( fs::initial_path().string() );
    relative.erase( 0, boost_root.string().size()+1 );    
    fs::path bin_path = find_bin_path(relative);

    report << "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">\n";

    // generate the column headings

    report << "<tr><td>Library</td><td>Test Name</td>\n"
      "<td><a href=\"compiler_status.html#test-type\">Test Type</a></td>\n";

    fs::directory_iterator itr( bin_path );
    while ( itr != end_itr 
      && ((itr->string().find( ".test" ) != (itr->string().size()-5))
      || !fs::is_directory( *itr )))
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
  fs::path comment_path;
  while ( argc > 1 && *argv[1] == '-' )
  {
    if ( argc > 2 && std::strcmp( argv[1], "--compiler" ) == 0 )
      { specific_compiler = argv[2]; --argc; ++argv; }
    else if ( argc > 2 && std::strcmp( argv[1], "--locate-root" ) == 0 )
      { locate_root = fs::path( argv[2], fs::native ); --argc; ++argv; }
    else if ( argc > 2 && std::strcmp( argv[1], "--comment" ) == 0 )
      { comment_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
    else if ( argc > 2 && std::strcmp( argv[1], "--notes" ) == 0 )
      { notes_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
    else if ( argc > 2 && std::strcmp( argv[1], "--notes-map" ) == 0 )
      { notes_map_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
    else if ( std::strcmp( argv[1], "--ignore-pass" ) == 0 ) ignore_pass = true;
    else if ( std::strcmp( argv[1], "--no-warn" ) == 0 ) no_warn = true;
    else if ( std::strcmp( argv[1], "--v2" ) == 0 ) boost_build_v2 = true;
    else if ( argc > 2 && std::strcmp( argv[1], "--jamfile" ) == 0)
      { jamfile_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
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
      "           --comment path      Path to file containing HTML\n"
      "                               to be copied into status-file.\n"
      "           --notes path        Path to file containing HTML\n"
      "                               to be copied into status-file.\n"
      "           --notes-map path    Path to file of toolset/test,n lines, where\n"
      "                               n is number of note bookmark in --notes file.\n"
      "           --jamfile path      Path to Jamfile. By default \"Jamfile\".\n"
      "Example: compiler_status --compiler gcc /boost-root cs.html cs-links.html\n"
      "Note: Only the leaf of the links-file path and --notes file string are\n"
      "used in status-file HTML links. Thus for browsing, status-file,\n"
      "links-file, and --notes file must all be in the same directory.\n"
      ;
    return 1;
  }

  boost_root = fs::path( argv[1], fs::native );
  if ( locate_root.empty() ) locate_root = boost_root;
  
  if (jamfile_path.empty())
    if (boost_build_v2)
      jamfile_path = "Jamfile.v2";
    else
      jamfile_path = "Jamfile";
  jamfile_path = fs::complete( jamfile_path, fs::initial_path() );
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

  build_notes_bookmarks();

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
          "<td><img border=\"0\" src=\"../boost.png\" width=\"277\" "
          "height=\"86\"></td>\n"
          "<td>\n"
          "<h1>Compiler Status: " + platform_desc() + "</h1>\n"
          "<b>Run Date:</b> "
       << run_date
       << "\n"
       ;

  if ( !comment_path.empty() )
  {
    fs::ifstream comment_file( comment_path );
    if ( !comment_file )
    {
      std::cerr << "Could not open \"--comment\" input file: " << comment_path.string() << std::endl;
      return 1;
    }
    char c;
    while ( comment_file.get( c ) ) { report.put( c ); }
  }

  report << "</td>\n</table>\n<br>\n";

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
         "<td><img border=\"0\" src=\"../boost.png\" width=\"277\" "
         "height=\"86\"></td>\n"
         "<td>\n"
         "<h1>Compiler Status: " + platform_desc() + "</h1>\n"
         "<b>Run Date:</b> "
      << run_date
      << "\n</td>\n</table>\n<br>\n"
      ;
  }

  do_table();

  if ( load_notes_html() ) report << notes_html << "\n";

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
