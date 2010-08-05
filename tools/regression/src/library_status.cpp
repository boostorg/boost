//  Generate Compiler Status HTML from jam regression test output  -----------//

//  Copyright Beman Dawes 2002.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/tools/regression/ for documentation.

//Note: This version of the original program builds a large table
//which includes all build variations such as build/release, static/dynamic, etc.


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

namespace fs = boost::filesystem;

#include "detail/tiny_xml.hpp"
namespace xml = boost::tiny_xml;

#include "boost/iterator/transform_iterator.hpp"

#include <cstdlib>  // for abort, exit
#include <string>
#include <vector>
#include <set>
#include <utility>  // for make_pair on STLPort
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <cassert>
#include <utility>

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
    bool ignore_pass = false;
    bool no_warn = false;
    bool no_links = false;

    fs::directory_iterator end_itr;

    // transform pathname to something html can accept
    struct char_xlate {
        typedef char result_type;
        result_type operator()(char c) const{
            if(c == '/')
                return '-';
            return c;
        }
    };
    typedef boost::transform_iterator<char_xlate, std::string::const_iterator> html_from_path; 

    template<class I1, class I2>
    std::ostream & operator<<(
    std::ostream &os, 
    std::pair<I1, I2> p
    ){
        while(p.first != p.second)
            os << *p.first++;
        return os;
    }

    struct col_node {
        int rows, cols;
        bool has_leaf;
        typedef std::map<std::string, col_node> subcolumns_t;
        subcolumns_t m_subcolumns;
        bool operator<(const col_node &cn) const;
        col_node() :
            has_leaf(false)
        {}
        std::pair<int, int> get_spans();
    };

    std::pair<int, int> col_node::get_spans(){
        rows = 1;
        cols = 0;
        if(has_leaf){
            cols = 1;
        }
        if(! m_subcolumns.empty()){
            subcolumns_t::iterator itr;
            for(itr = m_subcolumns.begin(); itr != m_subcolumns.end(); ++itr){
                std::pair<int, int> spans;
                spans = itr->second.get_spans();
                rows = (std::max)(rows, spans.first);
                cols += spans.second;
            }
            ++rows;
        }
        return std::make_pair(rows, cols);
    }

    void build_node_tree(const fs::path & dir_root, col_node & node){
        fs::path xml_file_path( dir_root / "test_log.xml" );
        if (fs::exists( xml_file_path ) )
        {
            node.has_leaf = true;
        }
        fs::directory_iterator itr(dir_root);
        while(itr != end_itr){
            if(fs::is_directory(*itr)){
                std::pair<col_node::subcolumns_t::iterator, bool> result 
                    = node.m_subcolumns.insert(
                        std::make_pair(itr->filename(), col_node())
                    );
                build_node_tree(*itr, result.first->second);
            }
            ++itr;
        }
    }

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

    //  element_content  ---------------------------------------------------------//

    const string & element_content(
        const xml::element & root, const string & name )
    {
        const static string empty_string;
        xml::element_list::const_iterator itr;
        for ( itr = root.elements.begin();
            itr != root.elements.end() && (*itr)->name != name;
            ++itr ) {}
            return itr != root.elements.end() ? (*itr)->content : empty_string;
    }

    //  find_element  ------------------------------------------------------------//

    const xml::element & find_element(
        const xml::element & root, const string & name )
    {
        static const xml::element empty_element;
        xml::element_list::const_iterator itr;
        for ( itr = root.elements.begin();
            itr != root.elements.end() && (*itr)->name != name;
            ++itr ) {}
            return itr != root.elements.end() ? *((*itr).get()) : empty_element;
    }

    //  attribute_value  ----------------------------------------------------------//

    const string & attribute_value( 
        const xml::element & element,
        const string & attribute_name 
    ){
        xml::attribute_list::const_iterator atr;
        for(
            atr = element.attributes.begin();
            atr != element.attributes.end();
            ++atr
        ){
            if(atr->name == attribute_name)
                return atr->value;
        }
        static const string empty_string;
        return empty_string;
    }

    //  generate_report  ---------------------------------------------------------//

    // return 0 if nothing generated, 1 otherwise, except 2 if compiler msgs
    int generate_report( 
        const xml::element & db,
        const std::string source_library_name,
        const string & test_type,
        const fs::path & target_dir,
        bool pass,
        bool always_show_run_output 
        )
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
            && (
                compile.empty() || test_type == "compile_fail"
            ) 
            && link.empty() 
            && run.empty()
        ) 
            return 0; 

        int result = 1; // some kind of msg for sure

        // limit compile message length
        if ( compile.size() > max_compile_msg_size )
        {
            compile.erase( max_compile_msg_size );
            compile += "...\n   (remainder deleted because of excessive size)\n";
        }

        links_file << "<h2><a name=\"";
        links_file << std::make_pair(
            html_from_path(target_dir.string().begin()), 
            html_from_path(target_dir.string().end())
            )
            << "\">"
            << std::make_pair(
            html_from_path(target_dir.string().begin()), 
            html_from_path(target_dir.string().end())
            )
            ;
        links_file << "</a></h2>\n";;

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
                << object_library_name << "-" 
                << std::make_pair(
                html_from_path(target_dir.string().begin()), 
                html_from_path(target_dir.string().end())
                )
                << source_library_name << " - "
                << object_library_name << " - " 
                << std::make_pair(
                html_from_path(target_dir.string().begin()), 
                html_from_path(target_dir.string().end())
                )
                << "</a>";
            if ( failed_lib_target_dirs.find( lib ) == failed_lib_target_dirs.end() )
            {
                failed_lib_target_dirs.insert( lib );
                fs::path pth( locate_root / lib / "test_log.xml" );
                fs::ifstream file( pth );
                if ( file )
                {
                    xml::element_ptr db = xml::parse( file, pth.string() );
                    generate_report( 
                        *db, 
                        source_library_name, 
                        test_type,
                        target_dir,
                        false,
                        false
                    );
                }
                else
                {
                    links_file << "<h2><a name=\""
                        << object_library_name << "-" 
                        << std::make_pair(
                        html_from_path(target_dir.string().begin()), 
                        html_from_path(target_dir.string().end())
                        )
                        << "\">"
                        << object_library_name << " - " 
                        << std::make_pair(
                        html_from_path(target_dir.string().begin()), 
                        html_from_path(target_dir.string().end())
                        )
                        << "</a></h2>\n"
                        << "test_log.xml not found\n";
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

    //  do_cell  ---------------------------------------------------------------//
    bool do_cell(
        const fs::path & target_dir,
        const string & lib_name,
        string & target,
        bool profile
    ){
        // return true if any results except pass_msg
        bool pass = false;

        fs::path xml_file_path( target_dir / "test_log.xml" );
        if ( !fs::exists( xml_file_path ) )
        {
            // suppress message because there are too many of them.
            // "missing" is a legitmate result as its not a requirement
            // that every test be run in every figuration.
            //std::cerr << "Missing jam_log.xml in target:\n "
            //    << target_dir.string() << "\n";
            target += "<td align=\"right\">" + missing_residue_msg + "</td>";
            return true;
        }

        int anything_generated = 0;
        bool note = false;

        fs::ifstream file( xml_file_path );
        if ( !file ) // could not open jam_log.xml
        {
            std::cerr << "Can't open jam_log.xml in target:\n "
                << target_dir.string() << "\n";
            target += "<td>" + missing_residue_msg + "</td>";
            return false;
        }

        string test_type( "unknown" );
        bool always_show_run_output( false );

        xml::element_ptr dbp = xml::parse( file, xml_file_path.string() );
        const xml::element & db( *dbp );
        test_type = attribute_value( db, "test-type" );
        always_show_run_output
            = attribute_value( db, "show-run-output" ) == "true";

        std::string test_type_base( test_type );
        if ( test_type_base.size() > 5 )
        {
            const string::size_type trailer = test_type_base.size() - 5;
            if ( test_type_base.substr( trailer ) == "_fail" )
            {
                test_type_base.erase( trailer );
            }
        }
        if ( test_type_base.size() > 4 )
        {
            const string::size_type trailer = test_type_base.size() - 4;
            if ( test_type_base.substr( trailer ) == "_pyd" )
            {
                test_type_base.erase( trailer );
            }
        }
        const xml::element & test_type_element( find_element( db, test_type_base ) );

        pass = !test_type_element.name.empty()
            && attribute_value( test_type_element, "result" ) != "fail";

        if (!no_links){
            if(!test_type_element.name.empty())
                note = attribute_value( test_type_element, "result" ) == "note";
            anything_generated = 
                generate_report(
                db, 
                lib_name, 
                test_type,
                target_dir,
                pass,
                always_show_run_output || note 
            );
        }

        // generate the status table cell pass/warn/fail HTML
        target += "<td align=\"right\">";
        if ( anything_generated != 0 )
        {
            target += "<a href=\"";
            target += links_name;
            target += "#";
            std::copy(
                html_from_path(target_dir.string().begin()), 
                html_from_path(target_dir.string().end()),
                std::back_inserter(target)
                );
            target += "\">";
            target += pass
                ? (anything_generated < 2 ? pass_msg : warn_msg)
                : fail_msg;
            target += "</a>";
            if ( pass && note ) target += note_msg;
        }
        else  target += pass ? pass_msg : fail_msg;

        // if profiling
        if(profile && pass){
            // add link to profile
            target += " <a href=\"";
            target += (target_dir / "profile.txt").string();
            target += "\"><i>Profile</i></a>";
        }
        
        // if notes, generate the superscript HTML
//        if ( !notes.empty() ) 
//            target += get_notes( toolset, lib_name, test_name, !pass );

        target += "</td>";
        return (anything_generated != 0) || !pass;
    }

    bool visit_node_tree(
        const col_node & node,
        fs::path dir_root,
        const string & lib_name,
        string & target,
        bool profile
    ){
        bool retval = false;
        if(node.has_leaf){
            retval = do_cell(
                dir_root,
                lib_name,
                target,
                profile
            );
        }

        col_node::subcolumns_t::const_iterator col_itr;
        for(
            col_itr = node.m_subcolumns.begin(); 
            col_itr != node.m_subcolumns.end();
            ++col_itr
        ){
            fs::path subdir = dir_root / col_itr->first;
            retval |= visit_node_tree(
                col_itr->second, 
                subdir,
                lib_name,
                target,
                col_itr->first == "profile"
            );
        }
        return retval;
    }

    // emit results for each test
    void do_row(
        col_node test_node,
        const fs::path & test_dir,
        const string & lib_name,
        const string & test_name,
        string & target 
    ){
        string::size_type row_start_pos = target.size();

        target += "<tr>";

        target += "<td>";
        //target += "<a href=\"" + url_prefix_dir_view + "/libs/" + lib_name + "\">";
        target += test_name;
        target += "</a>";
        target += "</td>";

//        target += "<td>" + test_type + "</td>";

        bool no_warn_save = no_warn;

//        if ( test_type.find( "fail" ) != string::npos ) no_warn = true;

        // emit cells on this row
        bool anything_to_report = visit_node_tree(
            test_node, 
            test_dir,
            lib_name,
            target,
            false
        );

        target += "</tr>";

        if ( ignore_pass 
        && ! anything_to_report ) 
            target.erase( row_start_pos );

        no_warn = no_warn_save;
    }

    //  do_table_body  -----------------------------------------------------------//

    void do_table_body(
        col_node root_node, 
        const string & lib_name,
        const fs::path & test_lib_dir 
    ){
        // rows are held in a vector so they can be sorted, if desired.
        std::vector<string> results;

        for ( fs::directory_iterator itr( test_lib_dir ); itr != end_itr; ++itr )
        {
            if(! fs::is_directory(*itr))
                continue;
            
            string test_name = itr->filename();
            // if the file name contains ".test"
            string::size_type s = test_name.find( ".test" );
            if(string::npos != s)
                // strip it off
                test_name.resize(s);
            else
                // if it doesn't - skip this directory
                continue;

            results.push_back( std::string() ); 
            do_row(
                root_node, //*test_node_itr++,
                *itr, // test dir
                lib_name,
                test_name,
                results[results.size()-1] 
            );
        }

        std::sort( results.begin(), results.end() );

        for ( 
            std::vector<string>::iterator v(results.begin());
            v != results.end(); 
            ++v 
        ){ 
            report << *v << "\n"; 
        }
    }

    //  column header-----------------------------------------------------------//
    int header_depth(const col_node & root){
        col_node::subcolumns_t::const_iterator itr;
        int max_depth = 1;
        for(itr = root.m_subcolumns.begin(); itr != root.m_subcolumns.end(); ++itr){
            max_depth = (std::max)(max_depth, itr->second.rows);
        }
        return max_depth;
    }

    void header_cell(int rows, int cols, const std::string & name){
        // add row cells
        report << "<td align=\"center\" " ;
        if(1 < cols)
            report << "colspan=\"" << cols << "\" " ;
        if(1 < rows)
            // span rows to the end the header
            report << "rowspan=\"" << rows << "\" " ;
        report << ">" ;
        report << name;
        report << "</td>\n";
    }

    void emit_column_headers(
        const col_node & node, 
        int display_row, 
        int current_row,
        int row_count
    ){
        if(current_row < display_row){
            if(! node.m_subcolumns.empty()){
                col_node::subcolumns_t::const_iterator itr;
                for(itr = node.m_subcolumns.begin(); itr != node.m_subcolumns.end(); ++itr){
                    emit_column_headers(itr->second, display_row, current_row + 1, row_count);
                }
            }
            return;
        }
        if(node.has_leaf && ! node.m_subcolumns.empty()){
            header_cell(row_count - current_row, 1, std::string(""));
        }

        col_node::subcolumns_t::const_iterator itr;
        for(itr = node.m_subcolumns.begin(); itr != node.m_subcolumns.end(); ++itr){
            if(1 == itr->second.rows)
                header_cell(row_count - current_row, itr->second.cols, itr->first);
            else
                header_cell(1, itr->second.cols, itr->first);
        }
    }

    fs::path find_lib_test_dir(){
        // walk up from the path were we started until we find
        // bin or bin.v2

        fs::path::const_iterator leaf_itr = fs::initial_path().end();
        fs::path test_lib_dir = fs::initial_path();
        for(;;){
            if(fs::is_directory( test_lib_dir / "bin.v2")){
                test_lib_dir /= "bin.v2";
                break;
            }
            if(fs::is_directory( test_lib_dir / "bin")){
                // v1 includes the word boost
                test_lib_dir /= "bin";
                test_lib_dir /= "boost";
                break;
            }
            if(test_lib_dir.empty())
                throw std::string("binary path not found");
            if(*leaf_itr != "libs")
                --leaf_itr;
            test_lib_dir.remove_filename();
        }

        if(leaf_itr == fs::initial_path().end())
            throw std::string("must be run from within a library directory");

        while(leaf_itr != fs::initial_path().end()){
            test_lib_dir /= *leaf_itr++;    // append "libs"
        }
        return test_lib_dir;
    }

    // note : uncomment the #if/#endif and what this compile !!!
    string find_lib_name(fs::path lib_test_dir){
        unsigned int count;
        fs::path::iterator e_itr = lib_test_dir.end();
        for(count =  0;; ++count){
            if(*--e_itr == "libs")
                break;
            if(lib_test_dir.empty())
                throw std::string("must be run from within a library directory");
        }
        string library_name;
        for(;;){
            library_name.append(*++e_itr);
            if(1 == --count)
                break;
            library_name.append("/");
        }
        return library_name;
    }

    fs::path find_boost_root(){
        fs::path boost_root = fs::initial_path();
        for(;;){
            if(fs::is_directory( boost_root / "boost")){
                break;
            }
            if(boost_root.empty())
                throw std::string("boost root not found");
            boost_root.remove_filename();
        }

        return boost_root;
    }

    //  do_table  ----------------------------------------------------------------//
    void do_table(const string & lib_name)
    {
        col_node root_node;

        fs::path lib_test_dir = find_lib_test_dir();

        for ( fs::directory_iterator itr(lib_test_dir); itr != end_itr; ++itr )
        {
            if(! fs::is_directory(*itr))
                continue;
            build_node_tree(*itr, root_node);
        }

        // visit directory nodes and record nodetree
        report << "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">\n";

        // emit
        root_node.get_spans();
        int row_count = header_depth(root_node);
        report << "<tr>\n";
        report << "<td rowspan=\"" << row_count << "\">Test Name</td>\n";

        // emit column headers
        int row_index = 0;
        for(;;){
            emit_column_headers(root_node, row_index, 0, row_count);
            report << "</tr>" ;
            if(++row_index == row_count)
                break;
            report << "<tr>\n";
        }

        // now the rest of the table body
        do_table_body(root_node, lib_name, lib_test_dir);

        report << "</table>\n";
   }
}// unnamed namespace

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
        else if ( argc > 2 && std::strcmp( argv[1], "--boost-root" ) == 0 )
        { boost_root = fs::path( argv[2], fs::native ); --argc; ++argv; }
        else if ( argc > 2 && std::strcmp( argv[1], "--comment" ) == 0 )
        { comment_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
        else if ( argc > 2 && std::strcmp( argv[1], "--notes" ) == 0 )
        { notes_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
        else if ( argc > 2 && std::strcmp( argv[1], "--notes-map" ) == 0 )
        { notes_map_path = fs::path( argv[2], fs::native ); --argc; ++argv; }
        else if ( std::strcmp( argv[1], "--ignore-pass" ) == 0 ) ignore_pass = true;
        else if ( std::strcmp( argv[1], "--no-warn" ) == 0 ) no_warn = true;
        else if ( std::strcmp( argv[1], "--v2" ) == 0 )
        {--argc; ++argv ;} // skip
        else if ( argc > 2 && std::strcmp( argv[1], "--jamfile" ) == 0)
        {--argc; ++argv;} // skip
        else { std::cerr << "Unknown option: " << argv[1] << "\n"; argc = 1; }
        --argc;
        ++argv;
    }

    if ( argc != 2 && argc != 3 )
    {
        std::cerr <<
            "Usage: library_status [options...] status-file [links-file]\n"
            "  boost-root is the path to the boost tree root directory.\n"
            "  status-file and links-file are paths to the output files.\n"
            "  options: --compiler name     Run for named compiler only\n"
            "           --ignore-pass       Do not report tests which pass all compilers\n"
            "           --no-warn           Warnings not reported if test passes\n"
            "           --boost-root path default derived from current path.\n"
            "           --locate-root path  Path to ALL_LOCATE_TARGET for bjam;\n"
            "                               default boost-root.\n"
            "           --comment path      Path to file containing HTML\n"
            "                               to be copied into status-file.\n"
            "           --notes path        Path to file containing HTML\n"
            "                               to be copied into status-file.\n"
            "           --notes-map path    Path to file of toolset/test,n lines, where\n"
            "                               n is number of note bookmark in --notes file.\n"
            "Example: compiler_status --compiler gcc /boost-root cs.html cs-links.html\n"
            "Note: Only the leaf of the links-file path and --notes file string are\n"
            "used in status-file HTML links. Thus for browsing, status-file,\n"
            "links-file, and --notes file must all be in the same directory.\n"
            ;
        return 1;
    }

    if(boost_root.empty())
        boost_root = find_boost_root();
    if ( locate_root.empty() ) 
        locate_root = boost_root;

    report.open( fs::path( argv[1], fs::native ) );
    if ( !report )
    {
        std::cerr << "Could not open report output file: " << argv[2] << std::endl;
        return 1;
    }

    if ( argc == 3 )
    {
        fs::path links_path( argv[2], fs::native );
        links_name = links_path.filename();
        links_file.open( links_path );
        if ( !links_file )
        {
            std::cerr << "Could not open links output file: " << argv[3] << std::endl;
            return 1;
        }
    }
    else no_links = true;

    build_notes_bookmarks();

    const string library_name = find_lib_name(fs::initial_path());

    char run_date[128];
    std::time_t tod;
    std::time( &tod );
    std::strftime( run_date, sizeof(run_date),
        "%X UTC, %A %d %B %Y", std::gmtime( &tod ) );

    report 
        << "<html>\n"
        << "<head>\n"
        << "<title>Boost Library Status Automatic Test</title>\n"
        << "</head>\n"
        << "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
        << "<table border=\"0\">\n"
        << "<tr>\n"
        << "<td><img border=\"0\" " 
        << "src=\""
        << boost_root / "boost.png"
        << "\" width=\"277\" "
        << "height=\"86\"></td>\n"
        << "<td>\n"
        << "<h1>Library Status: " + library_name + "</h1>\n"
        << "<b>Run Date:</b> "
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
            << "<head>\n"
            << "<title>Boost Library Status Error Log</title>\n"
            << "</head>\n"
            << "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
            << "<table border=\"0\">\n"
            << "<tr>\n"
            << "<td><img border=\"0\" src=\""
            << boost_root / "boost.png"
            << "\" width=\"277\" "
            << "height=\"86\"></td>\n"
            << "<td>\n"
            << "<h1>Library Status: " + library_name + "</h1>\n"
            << "<b>Run Date:</b> "
            << run_date
            << "\n</td>\n</table>\n<br>\n"
            ;
    }

    do_table(library_name);

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
