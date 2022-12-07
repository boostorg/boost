///////////////////////////////////////////////////////////////
//  Copyright 2012 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_MATH_FLOAT_BACKEND_HPP
#define BOOST_MATH_FLOAT_BACKEND_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <boost/lexical_cast.hpp>
#include <boost/math/concepts/real_concept.hpp>
#include <boost/multiprecision/number.hpp>
#include <boost/integer/common_factor_rt.hpp>
#include <boost/container_hash/hash.hpp>

namespace boost {
namespace multiprecision {
namespace backends {

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4389 4244 4018 4244 4127)
#endif

template <class Arithmetic>
struct arithmetic_backend
{
   typedef std::tuple<short, int, long, long long>                                 signed_types;
   typedef std::tuple<unsigned short, unsigned, unsigned long, unsigned long long> unsigned_types;
   typedef std::tuple<float, double, long double>                                  float_types;
   typedef int                                                                    exponent_type;

   BOOST_MP_CXX14_CONSTEXPR arithmetic_backend() : m_value(0) {}
   BOOST_MP_CXX14_CONSTEXPR arithmetic_backend(const arithmetic_backend& o) : m_value(o.m_value) {}
   template <class A>
   BOOST_MP_CXX14_CONSTEXPR arithmetic_backend(const A& o, const typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A>::value && std::numeric_limits<A>::is_specialized>::type* = 0) : m_value(o) {}
   template <class A>
   BOOST_MP_CXX14_CONSTEXPR arithmetic_backend(const arithmetic_backend<A>& o) : m_value(o.data()) {}
   BOOST_MP_CXX14_CONSTEXPR arithmetic_backend& operator=(const arithmetic_backend& o)
   {
      m_value = o.m_value;
      return *this;
   }
   template <class A>
   BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A>::value, arithmetic_backend&>::type operator=(A i)
   {
      m_value = static_cast<Arithmetic>(i);
      return *this;
   }
   template <class A>
   BOOST_MP_CXX14_CONSTEXPR arithmetic_backend& operator=(const arithmetic_backend<A>& i)
   {
      m_value = i.data();
      return *this;
   }
   arithmetic_backend& operator=(const char* s)
   {
#ifndef BOOST_NO_EXCEPTIONS
      try
      {
#endif
         m_value = boost::lexical_cast<Arithmetic>(s);
#ifndef BOOST_NO_EXCEPTIONS
      }
      catch (const bad_lexical_cast&)
      {
         throw std::runtime_error(std::string("Unable to interpret the string provided: \"") + s + std::string("\" as a compatible number type."));
      }
#endif
      return *this;
   }
   BOOST_MP_CXX14_CONSTEXPR void swap(arithmetic_backend& o)
   {
      std::swap(m_value, o.m_value);
   }
   std::string str(std::streamsize digits, std::ios_base::fmtflags f) const
   {
      std::stringstream ss;
      ss.flags(f);
      ss << std::setprecision(digits ? digits : std::numeric_limits<Arithmetic>::digits10 + 4) << m_value;
      return ss.str();
   }
   BOOST_MP_CXX14_CONSTEXPR void do_negate(const std::integral_constant<bool, true>&)
   {
      m_value = 1 + ~m_value;
   }
   BOOST_MP_CXX14_CONSTEXPR void do_negate(const std::integral_constant<bool, false>&)
   {
      m_value = -m_value;
   }
   BOOST_MP_CXX14_CONSTEXPR void negate()
   {
      do_negate(std::integral_constant<bool, boost::multiprecision::detail::is_unsigned<Arithmetic>::value>());
   }
   BOOST_MP_CXX14_CONSTEXPR int compare(const arithmetic_backend& o) const
   {
      return m_value > o.m_value ? 1 : (m_value < o.m_value ? -1 : 0);
   }
   template <class A>
   BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A>::value, int>::type compare(A i) const
   {
      return m_value > static_cast<Arithmetic>(i) ? 1 : (m_value < static_cast<Arithmetic>(i) ? -1 : 0);
   }
   BOOST_MP_CXX14_CONSTEXPR Arithmetic& data() { return m_value; }
   BOOST_MP_CXX14_CONSTEXPR const Arithmetic& data() const { return m_value; }

 private:
   Arithmetic m_value;
};

template <class R, class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_integral<R>::value>::type eval_convert_to(R* result, const arithmetic_backend<Arithmetic>& backend)
{
   typedef typename std::common_type<R, Arithmetic>::type c_type;
   constexpr const c_type                             max = static_cast<c_type>((std::numeric_limits<R>::max)());
   constexpr const c_type                             min = static_cast<c_type>((std::numeric_limits<R>::min)());
   c_type                                                   ct  = static_cast<c_type>(backend.data());
   if ((backend.data() < 0) && !std::numeric_limits<R>::is_signed)
      BOOST_THROW_EXCEPTION(std::range_error("Attempt to convert negative number to unsigned type."));
   if (ct > max)
      *result = boost::multiprecision::detail::is_signed<R>::value ? (std::numeric_limits<R>::max)() : backend.data();
   else if (std::numeric_limits<Arithmetic>::is_signed && (ct < min))
      *result = (std::numeric_limits<R>::min)();
   else
      *result = backend.data();
}

template <class R, class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<!boost::multiprecision::detail::is_integral<R>::value && !std::is_enum<R>::value>::type eval_convert_to(R* result, const arithmetic_backend<Arithmetic>& backend)
{
   *result = backend.data();
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR bool eval_eq(const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   return a.data() == b.data();
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value, bool>::type eval_eq(const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   return a.data() == static_cast<Arithmetic>(b);
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR bool eval_lt(const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   return a.data() < b.data();
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value, bool>::type eval_lt(const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   return a.data() < static_cast<Arithmetic>(b);
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR bool eval_gt(const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   return a.data() > b.data();
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value, bool>::type eval_gt(const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   return a.data() > static_cast<Arithmetic>(b);
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_add(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   result.data() += o.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_subtract(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   result.data() -= o.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_multiply(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   result.data() *= o.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<std::numeric_limits<Arithmetic>::has_infinity>::type eval_divide(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   result.data() /= o.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<!std::numeric_limits<Arithmetic>::has_infinity>::type eval_divide(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   if (!o.data())
      BOOST_THROW_EXCEPTION(std::overflow_error("Divide by zero"));
   result.data() /= o.data();
}

template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value>::type eval_add(arithmetic_backend<Arithmetic>& result, const A2& o)
{
   result.data() += o;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value>::type eval_subtract(arithmetic_backend<Arithmetic>& result, const A2& o)
{
   result.data() -= o;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value>::type eval_multiply(arithmetic_backend<Arithmetic>& result, const A2& o)
{
   result.data() *= o;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<(boost::multiprecision::detail::is_arithmetic<A2>::value && !std::numeric_limits<Arithmetic>::has_infinity)>::type
eval_divide(arithmetic_backend<Arithmetic>& result, const A2& o)
{
   if (!o)
      BOOST_THROW_EXCEPTION(std::overflow_error("Divide by zero"));
   result.data() /= o;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<(boost::multiprecision::detail::is_arithmetic<A2>::value && std::numeric_limits<Arithmetic>::has_infinity)>::type
eval_divide(arithmetic_backend<Arithmetic>& result, const A2& o)
{
   result.data() /= o;
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_add(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   result.data() = a.data() + b.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_subtract(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   result.data() = a.data() - b.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_multiply(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   result.data() = a.data() * b.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<std::numeric_limits<Arithmetic>::has_infinity>::type eval_divide(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   result.data() = a.data() / b.data();
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<!std::numeric_limits<Arithmetic>::has_infinity>::type eval_divide(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   if (!b.data())
      BOOST_THROW_EXCEPTION(std::overflow_error("Divide by zero"));
   result.data() = a.data() / b.data();
}

template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value>::type eval_add(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   result.data() = a.data() + b;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value>::type eval_subtract(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   result.data() = a.data() - b;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_arithmetic<A2>::value>::type eval_multiply(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   result.data() = a.data() * b;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<(boost::multiprecision::detail::is_arithmetic<A2>::value && !std::numeric_limits<Arithmetic>::has_infinity)>::type
eval_divide(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   if (!b)
      BOOST_THROW_EXCEPTION(std::overflow_error("Divide by zero"));
   result.data() = a.data() / b;
}
template <class Arithmetic, class A2>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<(boost::multiprecision::detail::is_arithmetic<A2>::value && std::numeric_limits<Arithmetic>::has_infinity)>::type
eval_divide(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const A2& b)
{
   result.data() = a.data() / b;
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR bool eval_is_zero(const arithmetic_backend<Arithmetic>& val)
{
   return val.data() == 0;
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<
    (!std::numeric_limits<Arithmetic>::is_specialized || std::numeric_limits<Arithmetic>::is_signed), int>::type
eval_get_sign(const arithmetic_backend<Arithmetic>& val)
{
   return val.data() == 0 ? 0 : val.data() < 0 ? -1 : 1;
}
template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<
    !(std::numeric_limits<Arithmetic>::is_specialized || std::numeric_limits<Arithmetic>::is_signed), int>::type
eval_get_sign(const arithmetic_backend<Arithmetic>& val)
{
   return val.data() == 0 ? 0 : 1;
}

template <class T>
inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<boost::multiprecision::detail::is_unsigned<T>::value, T>::type abs(T v) { return v; }

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_abs(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   using boost::multiprecision::backends::abs;
   using std::abs;
   result.data() = abs(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_fabs(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   result.data() = std::abs(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_floor(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = floor(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_ceil(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = ceil(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_sqrt(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = sqrt(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR int eval_fpclassify(const arithmetic_backend<Arithmetic>& o)
{
   return (boost::math::fpclassify)(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_trunc(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = trunc(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_round(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = round(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_frexp(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, int* v)
{
   BOOST_MATH_STD_USING
   result.data() = frexp(a.data(), v);
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_ldexp(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, int v)
{
   BOOST_MATH_STD_USING
   result.data() = ldexp(a.data(), v);
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_exp(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = exp(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_log(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = log(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_log10(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = log10(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_sin(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = sin(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_cos(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = cos(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_tan(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = tan(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_acos(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = acos(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_asin(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = asin(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_atan(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = atan(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_sinh(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = sinh(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_cosh(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = cosh(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_tanh(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& o)
{
   BOOST_MATH_STD_USING
   result.data() = tanh(o.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_fmod(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   BOOST_MATH_STD_USING
   result.data() = fmod(a.data(), b.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_pow(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   BOOST_MATH_STD_USING
   result.data() = pow(a.data(), b.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_atan2(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   BOOST_MATH_STD_USING
   result.data() = atan2(a.data(), b.data());
}

template <class Arithmetic, class I>
inline BOOST_MP_CXX14_CONSTEXPR void eval_left_shift(arithmetic_backend<Arithmetic>& result, I val)
{
   result.data() <<= val;
}

template <class Arithmetic, class I>
inline BOOST_MP_CXX14_CONSTEXPR void eval_right_shift(arithmetic_backend<Arithmetic>& result, I val)
{
   result.data() >>= val;
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_modulus(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a)
{
   result.data() %= a.data();
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_bitwise_and(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a)
{
   result.data() &= a.data();
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_bitwise_or(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a)
{
   result.data() |= a.data();
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_bitwise_xor(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a)
{
   result.data() ^= a.data();
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_complement(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a)
{
   result.data() = ~a.data();
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_gcd(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   result.data() = boost::integer::gcd(a.data(), b.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR void eval_lcm(arithmetic_backend<Arithmetic>& result, const arithmetic_backend<Arithmetic>& a, const arithmetic_backend<Arithmetic>& b)
{
   result.data() = boost::integer::lcm(a.data(), b.data());
}

template <class Arithmetic>
inline BOOST_MP_CXX14_CONSTEXPR std::size_t hash_value(const arithmetic_backend<Arithmetic>& a)
{
   boost::hash<Arithmetic> hasher;
   return hasher(a.data());
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

} // namespace backends

using boost::multiprecision::backends::arithmetic_backend;

template <class Arithmetic>
struct number_category<arithmetic_backend<Arithmetic> > : public std::integral_constant<int, boost::multiprecision::detail::is_integral<Arithmetic>::value ? number_kind_integer : number_kind_floating_point>
{};

namespace detail {

template <class Backend>
struct double_precision_type;

template <class Arithmetic, boost::multiprecision::expression_template_option ET>
struct double_precision_type<number<arithmetic_backend<Arithmetic>, ET> >
{
   typedef number<arithmetic_backend<typename double_precision_type<Arithmetic>::type>, ET> type;
};
template <>
struct double_precision_type<arithmetic_backend<std::int32_t> >
{
   typedef arithmetic_backend<std::int64_t> type;
};

} // namespace detail

}} // namespace boost::multiprecision
#if !(defined(__SGI_STL_PORT) || defined(BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS))
//
// We shouldn't need these to get code to compile, however for the sake of
// "level playing field" performance comparisons they avoid the very slow
// lexical_cast's that would otherwise take place.  Definition has to be guarded
// by the inverse of pp-logic in real_concept.hpp which defines these as a workaround
// for STLPort plus some other old/broken standartd libraries.
//
namespace boost { namespace math { namespace tools {

template <>
inline unsigned int real_cast<unsigned int, concepts::real_concept>(concepts::real_concept r)
{
   return static_cast<unsigned int>(r.value());
}

template <>
inline int real_cast<int, concepts::real_concept>(concepts::real_concept r)
{
   return static_cast<int>(r.value());
}

template <>
inline long real_cast<long, concepts::real_concept>(concepts::real_concept r)
{
   return static_cast<long>(r.value());
}

// Converts from T to narrower floating-point types, float, double & long double.

template <>
inline float real_cast<float, concepts::real_concept>(concepts::real_concept r)
{
   return static_cast<float>(r.value());
}
template <>
inline double real_cast<double, concepts::real_concept>(concepts::real_concept r)
{
   return static_cast<double>(r.value());
}
template <>
inline long double real_cast<long double, concepts::real_concept>(concepts::real_concept r)
{
   return r.value();
}

}}} // namespace boost::math::tools
#endif

namespace std {

template <class Arithmetic, boost::multiprecision::expression_template_option ExpressionTemplates>
class numeric_limits<boost::multiprecision::number<boost::multiprecision::arithmetic_backend<Arithmetic>, ExpressionTemplates> > : public std::numeric_limits<Arithmetic>
{
   typedef std::numeric_limits<Arithmetic>                                                                           base_type;
   typedef boost::multiprecision::number<boost::multiprecision::arithmetic_backend<Arithmetic>, ExpressionTemplates> number_type;

 public:
   static constexpr number_type(min)() noexcept { return (base_type::min)(); }
   static constexpr number_type(max)() noexcept { return (base_type::max)(); }
   static constexpr number_type lowest() noexcept { return -(max)(); }
   static constexpr number_type epsilon() noexcept { return base_type::epsilon(); }
   static constexpr number_type round_error() noexcept { return epsilon() / 2; }
   static constexpr number_type infinity() noexcept { return base_type::infinity(); }
   static constexpr number_type quiet_NaN() noexcept { return base_type::quiet_NaN(); }
   static constexpr number_type signaling_NaN() noexcept { return base_type::signaling_NaN(); }
   static constexpr number_type denorm_min() noexcept { return base_type::denorm_min(); }
};

template <>
class numeric_limits<boost::math::concepts::real_concept> : public std::numeric_limits<long double>
{
   typedef std::numeric_limits<long double>    base_type;
   typedef boost::math::concepts::real_concept number_type;

 public:
   static const number_type(min)() noexcept { return (base_type::min)(); }
   static const number_type(max)() noexcept { return (base_type::max)(); }
   static const number_type lowest() noexcept { return -(max)(); }
   static const number_type epsilon() noexcept { return base_type::epsilon(); }
   static const number_type round_error() noexcept { return epsilon() / 2; }
   static const number_type infinity() noexcept { return base_type::infinity(); }
   static const number_type quiet_NaN() noexcept { return base_type::quiet_NaN(); }
   static const number_type signaling_NaN() noexcept { return base_type::signaling_NaN(); }
   static const number_type denorm_min() noexcept { return base_type::denorm_min(); }
};

} // namespace std

#include <boost/multiprecision/detail/integer_ops.hpp>

#endif
