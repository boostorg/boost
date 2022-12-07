// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "boost/thread/scoped_thread.hpp"
#include "boost/thread.hpp"
#include <boost/core/lightweight_test.hpp>
#include <iostream>

static void inner_thread_func()
{
    boost::this_thread::sleep_for(
        boost::chrono::hours(1) );
}

static void outer_thread_func()
{
    boost::scoped_thread<boost::interrupt_and_join_if_joinable> inner( inner_thread_func );
}

static void double_interrupt()
{
    boost::scoped_thread<boost::interrupt_and_join_if_joinable> outer( outer_thread_func );
}

int main()
{
	BOOST_TEST( true ); // perform lwt initialization

    std::cout << "Start" << std::endl;
    double_interrupt();
    std::cout << "End" << std::endl;

	return boost::report_errors();
}
