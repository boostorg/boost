//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_SRC_ICU_UTIL_HPP
#define BOOST_SRC_ICU_UTIL_HPP

#include <boost/locale/config.hpp>
#ifdef BOOST_HAS_STDINT_H
#include <stdint.h> // Avoid ICU defining e.g. INT8_MIN causing macro redefinition warnings
#endif
#include <unicode/utypes.h>
#include <unicode/uversion.h>
#include <stdexcept>

#define BOOST_LOCALE_ICU_VERSION (U_ICU_VERSION_MAJOR_NUM * 100 + U_ICU_VERSION_MINOR_NUM)

namespace boost {
namespace locale {
namespace impl_icu {

    inline void throw_icu_error(UErrorCode err)
    {
        throw std::runtime_error(u_errorName(err));
    }

    inline void check_and_throw_icu_error(UErrorCode err)
    {
        if(U_FAILURE(err))
            throw_icu_error(err);
    }
} // impl
} // locale
} // boost

#endif
