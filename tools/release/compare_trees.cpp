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
      if ( file_size(*it) != file_size(t) )
      {
        std::cout << t.string() << " different size\n";
      }

      //  size of the two files is the same, so compare contents
      else
      {
        string contents1, contents2;
        load_file(*it, contents1);
        load_file(t, contents2);
        if ( contents1 != contents2 )
        {
          std::cout << t.string() << " different content\n";
        }
      }
    }
  }

  return 0;
}
