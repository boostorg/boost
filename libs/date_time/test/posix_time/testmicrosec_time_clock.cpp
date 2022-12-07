/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

 */

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/microsec_time_clock.hpp"
#include "../testfrmwk.hpp"
#if defined(BOOST_HAS_FTIME)
#include <windows.h>
#endif

void
sync_to_next_second()
{
  using namespace boost::posix_time;

  ptime t_prev;
  ptime t_now = second_clock::local_time();

  // Wait the next seconds
  do
  {
    t_prev = t_now;
    t_now = second_clock::local_time();
  } while (t_now.time_of_day().seconds() == t_prev.time_of_day().seconds());

  // Wait 300ms in order to avoid seconds of second_clock > microsec_clock.
  t_now = microsec_clock::local_time();
  t_prev = t_now;
  do
  {
    t_now = microsec_clock::local_time();
  } while (t_now - t_prev < milliseconds(300));

}


int
main()
{
#ifdef BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK

  using namespace boost::posix_time;

  std::cout << "Check local time of microsec_clock against second_clock" << std::endl;

  ptime last = microsec_clock::local_time();
  int max = 5;
  for (int i=0; i<max; i++)
  {
    // Some systems loop too fast so "last is less" tests fail due to
    // 'last' & 't2' being equal. These calls slow it down enough to
    // make 'last' & 't2' different. Moreover, we must wait the next
    // second to avoid a change in hour, minute or second field
    // between acquisition of t1 and t2.
    sync_to_next_second();

    ptime t1 = second_clock::local_time();
    std::cout << t1 << std::endl;

    ptime t2 = microsec_clock::local_time();
    std::cout << t2 << std::endl;

    // sometimes on very slow systems (github actions) a long time 
    // can pass between syncing to the next second + 300ms (see previous
    // function) and asking for t2; therefore if the fractional seconds
    // of t2 are less than 300ms we skip that test loop assuming it
    // took too long
    time_duration t2_tod = t2.time_of_day();
    if (t2_tod.fractional_seconds() < (t2_tod.ticks_per_second() * 3) / 10) {
        std::cout << "SKIP :: we had a significant host processing delay" << std::endl;
        continue;
    }

    check("check equality of hours "
          "between second_clock and microsec_clock timestamps",
          t1.time_of_day().hours() == t2.time_of_day().hours());

    check("check equality of minutes "
          "between second_clock and microsec_clock timestamps",
          t1.time_of_day().minutes() == t2.time_of_day().minutes());

    check("check equality of seconds "
          "between second_clock and microsec_clock timestamps",
          t1.time_of_day().seconds() == t2.time_of_day().seconds());

    check("check equality of date"
          "between second_clock and microsec_clock timestamps",
          t1.date() == t2.date());

    if( !check("check that previous microsec_clock timestamp "
               "is less than the current", last < t2) ) {
      std::cout << last << " < " << t2 << std::endl;
    }

    last = t2;
  }


  std::cout << "Check universal time of microsec_clock against second_clock" << std::endl;
  max = 5;
  last = microsec_clock::universal_time();
  for (int i=0; i<max; i++)
  {
    // Some systems loop too fast so "last is less" tests fail due to
    // 'last' & 't2' being equal. These calls slow it down enough to
    // make 'last' & 't2' different. Moreover, we must wait the next
    // second to avoid a change in hour, minute or second field
    // between acquisition of t1 and t2.
    sync_to_next_second();

    ptime t1 = second_clock::universal_time();
    std::cout << t1 << std::endl;

    ptime t2 = microsec_clock::universal_time();
    std::cout << t2 << std::endl;

    // sometimes on very slow systems (github actions) a long time 
    // can pass between syncing to the next second + 300ms (see previous
    // function) and asking for t2; therefore if the fractional seconds
    // of t2 are less than 300ms we skip that test loop assuming it
    // took too long
    time_duration t2_tod = t2.time_of_day();
    if (t2_tod.fractional_seconds() < (t2_tod.ticks_per_second() * 3) / 10) {
        std::cout << "SKIP :: we had a significant host processing delay" << std::endl;
        continue;
    }

    check("check equality of hours "
          "between second_clock and microsec_clock timestamps",
          t1.time_of_day().hours() == t2.time_of_day().hours());

    check("check equality of minutes "
          "between second_clock and microsec_clock timestamps",
          t1.time_of_day().minutes() == t2.time_of_day().minutes());

    check("check equality of seconds "
          "between second_clock and microsec_clock timestamps",
          t1.time_of_day().seconds() == t2.time_of_day().seconds());

    check("check equality of date"
          "between second_clock and microsec_clock timestamps",
          t1.date() == t2.date());

    if( !check("check that previous microsec_clock timestamp "
               "is less than the current", last < t2) ) {
      std::cout << last << " < " << t2 << std::endl;
    }

    last = t2;
  }

#else
  check("Get time of day micro second clock not supported due to inadequate compiler/platform", false);
#endif
  return printTestStats();

}
