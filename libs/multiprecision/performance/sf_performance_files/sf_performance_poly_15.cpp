///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void poly_tests_15()
{
#ifdef TEST_CPP_DEC_FLOAT
   time_proc("Polynomial Evaluation (100 digit precision)", "cpp_dec_float_100", test_polynomial<cpp_dec_float_100>);
#endif
}
