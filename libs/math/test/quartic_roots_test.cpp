/*
 * Copyright Nick Thompson, 2021
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "math_unit_test.hpp"
#include <random>
#include <boost/math/tools/quartic_roots.hpp>
#ifdef BOOST_HAS_FLOAT128
#include <boost/multiprecision/float128.hpp>
using boost::multiprecision::float128;
#endif

using boost::math::tools::quartic_roots;
using std::cbrt;
using std::sqrt;

template<class Real>
void test_zero_coefficients()
{
    Real a = 0;
    Real b = 0;
    Real c = 0;
    Real d = 0;
    Real e = 0;
    auto roots = quartic_roots(a,b,c,d,e);
    CHECK_EQUAL(roots[0], Real(0));
    CHECK_EQUAL(roots[1], Real(0));
    CHECK_EQUAL(roots[2], Real(0));
    CHECK_EQUAL(roots[3], Real(0));

    b = 1;
    e = 1;
    // x^3 + 1 = 0:
    roots = quartic_roots(a,b,c,d,e);
    CHECK_EQUAL(roots[0], Real(-1));
    CHECK_NAN(roots[1]);
    CHECK_NAN(roots[2]);
    CHECK_NAN(roots[3]);
    e = -1;
    // x^3 - 1 = 0:
    roots = quartic_roots(a,b,c,d,e);
    CHECK_EQUAL(roots[0], Real(1));
    CHECK_NAN(roots[1]);
    CHECK_NAN(roots[2]);
    CHECK_NAN(roots[3]);

    e = -2;
    // x^3 - 2 = 0
    roots = quartic_roots(a,b,c,d,e);
    CHECK_ULP_CLOSE(roots[0], cbrt(Real(2)), 2);
    CHECK_NAN(roots[1]);
    CHECK_NAN(roots[2]);
    CHECK_NAN(roots[3]);

    // x^4 -1 = 0
    // x = \pm 1:
    roots = quartic_roots<Real>(1, 0, 0, 0, -1);
    CHECK_ULP_CLOSE(Real(-1), roots[0], 3);
    CHECK_ULP_CLOSE(Real(1), roots[1], 3);
    CHECK_NAN(roots[2]);
    CHECK_NAN(roots[3]);

    // x^4 - 2 = 0 \implies x = \pm sqrt(sqrt(2))
    roots = quartic_roots<Real>(1,0,0,0,-2);
    CHECK_ULP_CLOSE(-sqrt(sqrt(Real(2))), roots[0], 3);
    CHECK_ULP_CLOSE(sqrt(sqrt(Real(2))), roots[1], 3);
    CHECK_NAN(roots[2]);
    CHECK_NAN(roots[3]);

    
    // x(x-1)(x-2)(x-3) = x^4 - 6x^3 + 11x^2 - 6x
    roots = quartic_roots(Real(1), Real(-6), Real(11), Real(-6), Real(0));
    CHECK_ULP_CLOSE(roots[0], Real(0), 2);
    CHECK_ULP_CLOSE(roots[1], Real(1), 2);
    CHECK_ULP_CLOSE(roots[2], Real(2), 2);
    CHECK_ULP_CLOSE(roots[3], Real(3), 2);

     // (x-1)(x-2)(x-3)(x-4) = x^4 - 10x^3 + 35x^2 - (2*3*4 + 1*3*4 + 1*2*4 + 1*2*3)x + 1*2*3*4  
    roots = quartic_roots<Real>(1, -10, 35, -24 - 12 - 8 - 6, 1*2*3*4);
    CHECK_ULP_CLOSE(Real(1), roots[0], 2);
    CHECK_ULP_CLOSE(Real(2), roots[1], 2);
    CHECK_ULP_CLOSE(Real(3), roots[2], 2);
    CHECK_ULP_CLOSE(Real(4), roots[3], 2);
    
    // Double root:
    // (x+1)^2(x-2)(x-3) = x^4 - 3x^3 -3x^2 + 7x + 6
    // Note: This test is unstable wrt to perturbations!
    roots = quartic_roots(Real(1), Real(-3), Real(-3), Real(7), Real(6));
    CHECK_ULP_CLOSE(Real(-1), roots[0], 2);
    CHECK_ULP_CLOSE(Real(-1), roots[1], 2);
    CHECK_ULP_CLOSE(Real(2), roots[2], 2);
    CHECK_ULP_CLOSE(Real(3), roots[3], 2);

     
    std::uniform_real_distribution<Real> dis(-2,2);
    std::mt19937 gen(12343);
    // Expected roots
    std::array<Real, 4> r;
    int trials = 10;
    for (int i = 0; i < trials; ++i) {
        // Mathematica:
        // Expand[(x - r0)*(x - r1)*(x - r2)*(x-r3)]
        // r0 r1 r2 r3 - (r0 r1 r2 + r0 r1 r3 + r0 r2 r3 + r1r2r3)x
        // + (r0 r1 + r0 r2 + r0 r3 + r1 r2 + r1r3 + r2 r3)x^2 - (r0 + r1 + r2 + r3) x^3 + x^4
        for (auto & root : r) {
            root = static_cast<Real>(dis(gen));
        }
        std::sort(r.begin(), r.end());
        Real a = 1;
        Real b = -(r[0] + r[1] + r[2] + r[3]);
        Real c = r[0]*r[1] + r[0]*r[2] + r[0]*r[3] + r[1]*r[2] + r[1]*r[3] + r[2]*r[3];
        Real d = -(r[0]*r[1]*r[2] + r[0]*r[1]*r[3] + r[0]*r[2]*r[3] + r[1]*r[2]*r[3]);
        Real e = r[0]*r[1]*r[2]*r[3];

        auto roots = quartic_roots(a, b, c, d, e);
        // I could check the condition number here, but this is fine right?
        CHECK_ULP_CLOSE(r[0], roots[0], 160);
        CHECK_ULP_CLOSE(r[1], roots[1], 260);
        CHECK_ULP_CLOSE(r[2], roots[2], 160);
        CHECK_ULP_CLOSE(r[3], roots[3], 160);
    }
}


int main()
{
    test_zero_coefficients<float>();
    test_zero_coefficients<double>();
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
    test_zero_coefficients<long double>();
#endif
    return boost::math::test::report_errors();
}
