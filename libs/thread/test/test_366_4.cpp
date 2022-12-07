// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "boost/thread/scoped_thread.hpp"
#include "boost/thread.hpp"
#include <boost/core/lightweight_test.hpp>
#include <iostream>

static void do_first_half()
{
    std::cout << "Doing first half of the work\n";

    boost::this_thread::sleep_for(
        boost::chrono::hours( 1 ) );

    std::cout << "First half of the work complete\n";
}

static void do_second_half()
{
    std::cout << "Doing second half of the work\n";

    boost::this_thread::sleep_for(
        boost::chrono::hours( 1 ) );

    std::cout << "Second half of the work complete\n";
}

static void outer_thread_func()
{
    boost::scoped_thread<boost::interrupt_and_join_if_joinable> worker1( do_first_half );
    boost::scoped_thread<boost::interrupt_and_join_if_joinable> worker2( do_second_half );

    worker1.join();
    worker2.join();
}

int main()
{
    BOOST_TEST( true ); // perform lwt initialization

    std::cout << "Start" << std::endl;

    {
        boost::thread outer( outer_thread_func );

        boost::this_thread::sleep_for( boost::chrono::seconds( 1 ) );

        std::cout << "Interrupting work\n";

        outer.interrupt();
        outer.join();
    }

    std::cout << "End" << std::endl;

    return boost::report_errors();
}
