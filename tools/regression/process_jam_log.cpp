//  process jam regression test output into XML  -----------------------------//

//  (C) Copyright Beman Dawes 2002. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

#include "detail/tiny_xml.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"

#include <iostream>
#include <string>
#include <ctime>

using std::string;
namespace xml = boost::tiny_xml;
namespace fs = boost::filesystem;

#define BOOST_NO_CPP_MAIN_SUCCESS_MESSAGE
#include <boost/test/included/prg_exec_monitor.hpp>

namespace
{
//  append_html  -------------------------------------------------------------//

  void append_html( const string & src, string & target )
  {
    // there are a few lines we want to ignore
    if ( src.find( "th target..." ) != string::npos
      || src.find( "cc1plus.exe: warning: changing search order for system directory" ) != string::npos
      || src.find( "cc1plus.exe: warning:   as it has already been specified as a non-system directory" ) != string::npos
      ) return;

    for ( string::size_type pos = 0; pos < src.size(); ++pos )
    {
      if ( src[pos] == '<' ) target += "&lt;";
      else if ( src[pos] == '>' ) target += "&gt;";
      else if ( src[pos] == '&' ) target += "&amp;";
      else target += src[pos];
    }
  }

 //  timestamp  ---------------------------------------------------------------//

  string timestamp()
  {
    char run_date[128];
    std::time_t tod;
    std::time( &tod );
    std::strftime( run_date, sizeof(run_date),
      "%Y-%m-%d %X UTC", std::gmtime( &tod ) );
    return string( run_date );
  }

//  convert path separators to forward slashes  ------------------------------//

  void convert_path_separators( string & s )
  {
    for ( string::iterator itr = s.begin(); itr != s.end(); ++itr )
      if ( *itr == '\\' || *itr == '!' ) *itr = '/';
  }

//  extract a directory from a jam target string  ----------------------------//

  string target_directory( const string & s )
  {
    string temp( s );
    convert_path_separators( temp );
    temp.erase( temp.find_last_of( "/" ) );
    string::size_type pos = temp.find_last_of( " " );
    if ( pos != string::npos ) temp.erase( 0, pos+1 );
    return temp;
  }

  string toolset( const string & s )
  {
    string t( s );
    string::size_type pos = t.find( "/bin/" );
    if ( pos != string::npos ) pos += 5;
    else return "";
    pos = t.find( "/", pos );
    if ( pos != string::npos ) pos += 1;
    else return "";
    return t.substr( pos, t.find( "/", pos ) - pos );
  }

  string test_name( const string & s )
  {
    string t( s );
    string::size_type pos = t.find( "/bin/" );
    if ( pos != string::npos ) pos += 5;
    else return "";
    return t.substr( pos, t.find( ".", pos ) - pos );
  }

  // the format of paths is really kinky, so convert to normal form
  //   first path is missing the leading "..\".
  //   first path is missing "\bin" after "status".
  //   second path is missing the leading "..\".
  //   second path is missing "\bin" after "build".
  //   second path uses "!" for some separators.
  void parse_skipped_msg( const string & msg,
    string & first_dir, string & second_dir )
  {
    first_dir.clear();
    second_dir.clear();
    string::size_type start_pos( msg.find( '<' ) );
    if ( start_pos == string::npos ) return;
    ++start_pos;
    string::size_type end_pos( msg.find( '>', start_pos ) );
    first_dir += msg.substr( start_pos, end_pos - start_pos );
    convert_path_separators( first_dir );
    first_dir.insert( 6, "/bin" );

    start_pos = msg.find( '<', end_pos );
    if ( start_pos == string::npos ) return;
    ++start_pos;
    end_pos = msg.find( '>', start_pos );
    second_dir += msg.substr( start_pos, end_pos - start_pos );
    convert_path_separators( second_dir );
    second_dir.insert( second_dir.find( "/build/" )+6, "/bin" );
  }



//  test_log hides database details  -----------------------------------------//

  class test_log
    : boost::noncopyable
  {
    const string & m_target_directory;
    xml::element_ptr m_root;
  public:
    test_log( const string & target_directory,
              const string & test_name,
              const string & toolset )
      : m_target_directory( target_directory )
    {
      fs::path pth( target_directory );
      pth /= "test_log.xml";
      fs::ifstream file( pth );
      if ( !file )
      {
        m_root.reset( new xml::element( "test-log" ) );
        m_root->attributes.push_back(
          xml::attribute( "target-directory", target_directory ) );
        m_root->attributes.push_back(
          xml::attribute( "test-name", test_name ) );
        m_root->attributes.push_back(
          xml::attribute( "toolset", toolset ) );
      }
      else // existing file
      {
        m_root = xml::parse( file );
      }
//std::cout <<     m_root->elements.size() << std::endl;
    }

    ~test_log()
    {
      fs::path pth( m_target_directory );
      pth /= "test_log.xml";
      fs::ofstream file( pth );
      if ( !file )
        throw fs::filesystem_error( "Can't open output file: "
        + pth.string(), fs::other_error );
      xml::write( *m_root, file );
    }

    const string & target_directory() const { return m_target_directory; }

    void remove_action( const string & action_name )
    // no effect if action_name not found
    {
      xml::element_list::iterator itr;
      for ( itr = m_root->elements.begin();
            itr != m_root->elements.end() && (*itr)->name != action_name;
            ++itr ) {}
      if ( itr != m_root->elements.end() ) m_root->elements.erase( itr );
    }

    void add_action( const string & action_name,
                     const string & result,
                     const string & timestamp,
                     const string & content )
    {
      remove_action( action_name );
      xml::element_ptr action( new xml::element(action_name) );
      m_root->elements.push_back( action );
      action->attributes.push_back( xml::attribute( "result", result ) );
      action->attributes.push_back( xml::attribute( "timestamp", timestamp ) );
      action->content = content;
    }
  };

//  message_manager maps input messages into test_log actions  ---------------//

  class message_manager
    : boost::noncopyable
  {
    string  m_action_name;  // !empty() implies action pending
                            // IOW, a start_message awaits stop_message
    string  m_target_directory;
    string  m_test_name;
    string  m_toolset;

  public:
    ~message_manager() { /*assert( m_action_name.empty() );*/ }

    void start_message( const string & action_name,
                      const string & target_directory,
                      const string & test_name,
                      const string & toolset,
                      const string & prior_content )
    {
      if ( !m_action_name.empty() ) stop_message( prior_content );
      m_action_name = action_name;
      m_target_directory = target_directory;
      m_test_name = test_name;
      m_toolset = toolset;
    }

    void stop_message( const string & content )
    {
      if ( m_action_name.empty() ) return;
      stop_message( m_action_name, m_target_directory,
        "succeed", timestamp(), content );
    }

    void stop_message( const string & action_name,
                     const string & target_directory,
                     const string & result,
                     const string & timestamp,
                     const string & content )
    // the only valid action_names are "compile", "link", "run", "lib"
    {
      // my understanding of the jam output is that there should never be
      // a stop_message that was not preceeded by a matching start_message.
      assert( m_action_name == action_name );
      assert( m_target_directory == target_directory );

      test_log tl( target_directory, m_test_name, m_toolset );

      // dependency removal
      if ( action_name == "lib" )
      {
        tl.remove_action( "compile" );
        tl.remove_action( "link" );
        tl.remove_action( "run" );
      }
      else if ( action_name == "compile" )
      {
        tl.remove_action( "link" );
        tl.remove_action( "run" );
      }
      else if ( action_name == "link" ) { tl.remove_action( "run" ); }

      // dependency removal won't work right with random names, so assert
      else { assert( action_name == "run" ); }

      // add the stop_message action
      tl.add_action( action_name, result, timestamp, content );

      m_action_name = ""; // signal no pending action
    }
  };
}

//  main  --------------------------------------------------------------------//

int cpp_main( int argc, char ** argv )
{
  message_manager mgr;

  string line;
  string content;
  bool capture_lines;

  while ( std::getline( std::cin, line ) )
  {
//std::cout << line << "\n";
    if ( line.find( "C++-action " ) != string::npos
      || line.find( "vc-C++ " ) != string::npos
      || line.find( "C-action " ) != string::npos
      || line.find( "Cc-action " ) != string::npos
      || line.find( "Link-action " ) != string::npos
      || line.find( "vc-Link " ) != string::npos )
    {
      string action( ( line.find( "Link-action " ) != string::npos
        || line.find( "vc-Link " ) != string::npos )
        ? "link" : "compile" );
      if ( line.find( "...failed " ) != string::npos )
        mgr.stop_message( action, target_directory( line ),
          "fail", timestamp(), content );
      else
      {
        string target_dir( target_directory( line ) );
        mgr.start_message( action, target_dir,
          test_name( target_dir ), toolset( target_dir ), content );
      }
      content = "\n";
      capture_lines = true;
    }

    else if ( line.find( "capture-run-output" ) != string::npos )
    {
      if ( line.find( "...failed " ) != string::npos )
      {
        mgr.stop_message( "run", target_directory( line ),
          "fail", timestamp(), content );
        content = "\n";
        capture_lines = true;
      }
      else
      {
        string target_dir( target_directory( line ) );
        mgr.start_message( "run", target_dir,
          test_name( target_dir ), toolset( target_dir ), content );

        // contents of .output file for content
        capture_lines = false;
        content = "\n";
        fs::ifstream file( fs::path(target_dir)
          / (test_name(target_dir) + ".output") );
        if ( file )
        {
          string ln;
          while ( std::getline( file, ln ) )
          {
            append_html( ln, content );;
            content += "\n";
          }
        }
      }
    }

    else if ( line.find( "...skipped <" ) != string::npos
      && line.find( ".test for lack of " ) != string::npos )
    {
      mgr.stop_message( content );
      content.clear();
      capture_lines = true;

      string target_dir;
      string lib_dir;

      parse_skipped_msg( line, target_dir, lib_dir );

      if ( target_dir != lib_dir ) // it's a lib problem
      {
        target_dir.insert( 0, "../" );
        mgr.start_message( "lib", target_dir, 
          test_name( target_dir ), toolset( target_dir ), content );
        content = lib_dir;
        mgr.stop_message( "lib", target_dir, "fail", timestamp(), content );
        content = "\n";
      }

    }

    else if ( line.find( "succeeded-test" ) != string::npos
      || line.find( "failed-test-file " ) != string::npos
      || line.find( "command-file-dump" ) != string::npos )
    {
      mgr.stop_message( content );
      content = "\n";
      capture_lines = true;
    }

    else if ( capture_lines ) // hang onto lines for possible later use
    {
      append_html( line, content );;
      content += "\n";
    }
  }

  mgr.stop_message( content );
  return 0;
}
