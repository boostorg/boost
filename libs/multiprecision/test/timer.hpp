///////////////////////////////////////////////////////////////
//  Copyright Beman Dawes 1994-99.
//  Copyright 2020 John Maddock.
//  Copyright 2022 Christopher Kormanyos.
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_MP_TIMER_HPP
#define BOOST_MP_TIMER_HPP

#include <chrono>

// This file now reflects the 2022 work-over. We have replaced the
// original timer facility (which had been based on std::clock())
// with C++11's equivalent <chrono> counterparts.

namespace boost { namespace multiprecision { namespace test_detail {

template <class ClockType = std::chrono::high_resolution_clock>
struct stopwatch;

template <class ResultType,
          class ClockType = std::chrono::high_resolution_clock>
class timer_template;

template <class ClockType>
struct stopwatch
{
private:
   using clock_type = ClockType;

public:
   using duration_type = typename clock_type::duration;

   stopwatch() : m_start(clock_type::now()) { }

   stopwatch(const stopwatch& other) : m_start(other.m_start) { }
   stopwatch(stopwatch&& other) noexcept : m_start(other.m_start) { }

   auto operator=(const stopwatch& other) -> stopwatch&
   {
      if(this != &other)
      {
         m_start = other.m_start;
      }

      return *this;
   }

   auto operator=(stopwatch&& other) noexcept -> stopwatch&
   {
      m_start = other.m_start;

      return *this;
   }

   ~stopwatch() { }

   auto elapsed() const -> duration_type
   {
      return (clock_type::now() - m_start);
   }

   auto elapsed_max() const -> duration_type
   {
      return (std::chrono::time_point<clock_type>::max)() - m_start;
   }

   static constexpr auto elapsed_min() -> duration_type
   {
      return (std::chrono::time_point<clock_type>::min)() -  std::chrono::time_point<clock_type>();
   }

   void reset()
   {
      m_start = clock_type::now();
   }

private:
   typename clock_type::time_point m_start;
};


template <class ResultType,
          class ClockType>
class timer_template : public stopwatch<ClockType>
{
private:
   using clock_type      = ClockType;
   using base_class_type = stopwatch<clock_type>;

public:
   using result_type = ResultType;

   timer_template() { }

   ~timer_template() { }

   void restart() { base_class_type::reset(); }

   auto elapsed() const -> result_type
   {
      // Return the elapsed time in seconds.
      return std::chrono::duration_cast<std::chrono::duration<result_type>>(base_class_type::elapsed()).count();
   }

   auto elapsed_max() const -> result_type
   {
      return std::chrono::duration_cast<std::chrono::duration<result_type>>(base_class_type::elapsed_max()).count();
   }

   static constexpr auto elapsed_min() -> result_type
   {
      return std::chrono::duration_cast<std::chrono::duration<result_type>>(base_class_type::elapsed_min()).count();
   }

   static constexpr result_type seconds(result_type s) { return static_cast<result_type>(s * static_cast<result_type>(1)); }

};

} // namespace test_detail
} // namespace multiprecision
} // namespace boost

// TODO: Might prefer to have the relatively common
// name "timer" to be shielded with a namespace.

using timer = boost::multiprecision::test_detail::timer_template<double, std::chrono::high_resolution_clock>;

#endif // BOOST_MP_TIMER_HPP
