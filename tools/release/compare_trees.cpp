//  compare_trees.cpp  -------------------------------------------------------//

//  Copyright Beman Dawes 2008

//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or
//  http://www.boost.org/LICENSE_1_0.txt)

//----------------------------------------------------------------------------//

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace boost::filesystem;
using std::string;

namespace
{

//  load_file  ---------------------------------------------------------------//

  void load_file( const path & p, string & target )
  {

    std::ifstream fin( p.string().c_str(),
      std::ios_base::in|std::ios_base::binary );
    if ( !fin )
      std::cout << "could not open input file: " << p.string() << '\n';
    std::getline( fin, target, '\0' ); // read the whole file
  }

//  svn_keyword  -------------------------------------------------------------//

  int svn_keyword( string::const_iterator beg, string::const_iterator end )
  {
    static const char k1 [] = {"LastChangeDate"};
    static const char k1a[] = {"Date"};
    static const char k2 [] = {"LastChangeRevision"};
    static const char k2a[] = {"Rev"};
    static const char k3 [] = {"LastChangedBy"};
    static const char k3a[] = {"Author"};
    static const char k4 [] = {"HeadURL"};
    static const char k4a[] = {"URL"};
    static const char k5 [] = {"Id"};

    std::size_t sz( end - beg );
    const char * p(&*beg);
    if ( sz > sizeof(k1) && memcmp(p, k1, sizeof(k1-1)) == 0 ) return 1;
    if ( sz > sizeof(k1a) && memcmp(p, k1a, sizeof(k1a-1)) == 0 ) return 1;
    if ( sz > sizeof(k2) && memcmp(p, k2, sizeof(k2-1)) == 0 ) return 2;
    if ( sz > sizeof(k2a) && memcmp(p, k2a, sizeof(k2a-1)) == 0 ) return 2;
    if ( sz > sizeof(k3) && memcmp(p, k3, sizeof(k3-1)) == 0 ) return 3;
    if ( sz > sizeof(k3a) && memcmp(p, k3a, sizeof(k3a-1)) == 0 ) return 3;
    if ( sz > sizeof(k4) && memcmp(p, k4, sizeof(k4-1)) == 0 ) return 4;
    if ( sz > sizeof(k4a) && memcmp(p, k4a, sizeof(k4a-1)) == 0 ) return 4;
    if ( sz > sizeof(k5) && memcmp(p, k5, sizeof(k5-1)) == 0 ) return 5;
    return 0;
  }

//  equivalent  --------------------------------------------------------------//
//  (ignoring SVN keyword contents) 

  bool equivalent( const string & s1, const string & s2 )
  {
    assert(s1.size() == s2.size());
    string::const_iterator it1(s1.begin()), end1(s1.end());
    string::const_iterator it2(s2.begin()), end2(s2.end());

    for ( ; it1 != end1 && it2 != end2; ++it1, ++it2 )
    {
      if ( *it1 != *it2 ) return false;
      int keyword;
      if ( *it1 == '$' && (keyword=svn_keyword(it1, end1)) != 0
        && keyword == svn_keyword(it2, end2) )
      {
        do { ++it1; } while ( it1 != end1 && *it1 != '$');
        do { ++it2; } while ( it2 != end2 && *it2 != '$');
      }
    }
    return true;
  }
}

//  main  --------------------------------------------------------------------//

int main( int argc, char * argv[] )
{
  if ( argc != 3 )
  {
    std::cout << "Usage: compare_trees [options ...] path1 path2\n";
    return 1;
  }

  path p1(argv[1]);
  path p2(argv[2]);

  recursive_directory_iterator it( p1 );
  recursive_directory_iterator end;

  for ( ; it != end; ++it )
  {
    //std::cout << it->string() << "\n";
    if ( it->filename()[0] == '.' )
    {
      it.no_push();
      continue;
    }
    string s = it->string();
    s.erase(0, p1.string().size());
    path t(p2);
    t /= s;
    if ( !exists(t) )
    {
      std::cout << t.string() << " does not exist\n";
      if ( is_directory( it.status() ) )
        it.no_push();
    }
    else if ( is_regular_file( it.status() ) )
    {
      string contents1, contents2;
      load_file(*it, contents1);
      load_file(t, contents2);
      if ( !equivalent( contents1, contents2 ) )
      {
        std::cout << t.string() << " different content\n";
      }
    }
  }

  return 0;
}
