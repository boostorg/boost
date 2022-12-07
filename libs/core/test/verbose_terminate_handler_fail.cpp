// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#if defined(_MSC_VER)
# pragma warning(disable: 4702) // unreachable code
# pragma warning(disable: 4530) // C++ exception handler used
# pragma warning(disable: 4577) // 'noexcept' used
#endif

#include <boost/core/verbose_terminate_handler.hpp>
#include <boost/throw_exception.hpp>
#include <exception>
#include <stdlib.h>
#if defined(_MSC_VER) && defined(_CPPLIB_VER) && defined(_DEBUG)
# include <crtdbg.h>
#endif

int main()
{
#if defined(_MSC_VER) && (_MSC_VER > 1310)
    // disable message boxes on assert(), abort()
    ::_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
#if defined(_MSC_VER) && defined(_CPPLIB_VER) && defined(_DEBUG)
    // disable message boxes on iterator debugging violations
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
#endif

    std::set_terminate( boost::core::verbose_terminate_handler );

    boost::throw_with_location( std::exception() );
}

#if defined(BOOST_NO_EXCEPTIONS)

void boost::throw_exception( std::exception const& x )
{
    std::fprintf( stderr, "throw_exception: %s\n\n", x.what() );
    std::terminate();
}

void boost::throw_exception( std::exception const& x, boost::source_location const& )
{
    std::fprintf( stderr, "throw_exception: %s\n\n", x.what() );
    std::terminate();
}

#endif
