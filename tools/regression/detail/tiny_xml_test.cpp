//  tiny XML test program  ---------------------------------------------------//

//  Copyright Beman Dawes 2002.
//  See accompanying license for terms and conditions of use.

#include "tiny_xml.hpp"

#include <iostream>

int main()
{
  boost::tiny_xml::element_ptr tree( boost::tiny_xml::parse( std::cin ) );
  boost::tiny_xml::write( *tree, std::cout );
  return 0;
}

