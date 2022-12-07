///////////////////////////////////////////////////////////////
//  Copyright 2021 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt
//

#include <Eigen/Dense>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/eigen.hpp>

typedef boost::multiprecision::cpp_rational               NT;
typedef Eigen::Matrix<NT, Eigen::Dynamic, Eigen::Dynamic> M;

void f(M& m1, M const& m2)
{
   m1 = m2 * m2;
}
