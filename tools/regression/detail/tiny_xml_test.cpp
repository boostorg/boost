//  tiny XML test program  ---------------------------------------------------//

//  (C) Copyright Beman Dawes 2002. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

#include "tiny_xml.hpp"

#include <iostream>

int main()
{
  boost::tiny_xml::element_ptr tree( boost::tiny_xml::parse( std::cin ) );
  boost::tiny_xml::write( *tree, std::cout );
  return 0;
}

