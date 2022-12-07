# Copyright 2020, 2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.20)

project(boost_locale VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

# Using glob here is ok as it is only for headers
file(GLOB_RECURSE headers include/*.hpp)

add_library(boost_locale
  src/boost/locale/encoding/codepage.cpp
  src/boost/locale/encoding/conv.hpp
  src/boost/locale/encoding/iconv_codepage.ipp
  src/boost/locale/encoding/uconv_codepage.ipp
  src/boost/locale/encoding/wconv_codepage.ipp
  src/boost/locale/shared/date_time.cpp
  src/boost/locale/shared/format.cpp
  src/boost/locale/shared/formatting.cpp
  src/boost/locale/shared/generator.cpp
  src/boost/locale/shared/ids.cpp
  src/boost/locale/shared/localization_backend.cpp
  src/boost/locale/shared/message.cpp
  src/boost/locale/shared/mo_lambda.cpp
  src/boost/locale/util/codecvt_converter.cpp
  src/boost/locale/util/default_locale.cpp
  src/boost/locale/util/info.cpp
  src/boost/locale/util/locale_data.cpp
  src/boost/locale/util/locale_data.hpp
  src/boost/locale/util/numeric.hpp
  src/boost/locale/util/timezone.hpp
  ${headers}
)

add_library(Boost::locale ALIAS boost_locale)

target_include_directories(boost_locale PUBLIC include PRIVATE src)

target_link_libraries(boost_locale
  PUBLIC
    Boost::assert
    Boost::config
    Boost::function
    Boost::iterator
    Boost::smart_ptr
    Boost::static_assert
    Boost::type_traits
  PRIVATE
    Boost::thread
    Boost::unordered
    Boost::predef
)

target_compile_definitions(boost_locale
  PUBLIC BOOST_LOCALE_NO_LIB
  PRIVATE _CRT_SECURE_NO_WARNINGS
)

if(BUILD_SHARED_LIBS)
  target_compile_definitions(boost_locale PUBLIC BOOST_LOCALE_DYN_LINK)
else()
  target_compile_definitions(boost_locale PUBLIC BOOST_LOCALE_STATIC_LINK)
endif()

# Build Options

find_package(Iconv QUIET) # requires CMake 3.11
find_package(ICU COMPONENTS data i18n uc QUIET)

set(_default_posix OFF)

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
  set(_default_posix ON)
endif()

option(BOOST_LOCALE_ENABLE_ICONV "Boost.Locale: enable iconv backend" ${ICONV_FOUND})
option(BOOST_LOCALE_ENABLE_ICU "Boost.Locale: enable ICU backend" ${ICU_FOUND})
option(BOOST_LOCALE_ENABLE_POSIX "Boost.Locale: enable POSIX backend" ${_default_posix})
option(BOOST_LOCALE_ENABLE_STD "Boost.Locale: enable std::locale backend" ON)
option(BOOST_LOCALE_ENABLE_WINAPI "Boost.Locale: enable Windows API backend" ${WIN32})

unset(_default_posix)

if(BOOST_LOCALE_ENABLE_ICONV)

  find_package(Iconv REQUIRED)

  target_compile_definitions(boost_locale PRIVATE BOOST_LOCALE_WITH_ICONV=1)
  target_link_libraries(boost_locale PRIVATE Iconv::Iconv)

  target_sources(boost_locale PRIVATE
    src/boost/locale/util/iconv.hpp
  )
endif()

if(BOOST_LOCALE_ENABLE_ICU)

  find_package(ICU COMPONENTS data i18n uc REQUIRED)

  target_compile_definitions(boost_locale PRIVATE BOOST_LOCALE_WITH_ICU=1)
  target_link_libraries(boost_locale PRIVATE ICU::data ICU::i18n ICU::uc)

  target_sources(boost_locale PRIVATE
    src/boost/locale/icu/all_generator.hpp
    src/boost/locale/icu/boundary.cpp
    src/boost/locale/icu/cdata.hpp
    src/boost/locale/icu/codecvt.cpp
    src/boost/locale/icu/codecvt.hpp
    src/boost/locale/icu/collator.cpp
    src/boost/locale/icu/conversion.cpp
    src/boost/locale/icu/date_time.cpp
    src/boost/locale/icu/formatter.cpp
    src/boost/locale/icu/formatter.hpp
    src/boost/locale/icu/icu_backend.cpp
    src/boost/locale/icu/icu_backend.hpp
    src/boost/locale/icu/icu_util.hpp
    src/boost/locale/icu/numeric.cpp
    src/boost/locale/icu/predefined_formatters.hpp
    src/boost/locale/icu/time_zone.cpp
    src/boost/locale/icu/time_zone.hpp
    src/boost/locale/icu/uconv.hpp
  )

endif()

if(BOOST_LOCALE_ENABLE_STD)

  target_sources(boost_locale PRIVATE
    src/boost/locale/std/all_generator.hpp
    src/boost/locale/std/codecvt.cpp
    src/boost/locale/std/collate.cpp
    src/boost/locale/std/converter.cpp
    src/boost/locale/std/numeric.cpp
    src/boost/locale/std/std_backend.cpp
    src/boost/locale/std/std_backend.hpp
  )

else()

  target_compile_definitions(boost_locale PRIVATE BOOST_LOCALE_NO_STD_BACKEND=1)

endif()

if(BOOST_LOCALE_ENABLE_WINAPI)

  target_sources(boost_locale PRIVATE
    src/boost/locale/win32/all_generator.hpp
    src/boost/locale/win32/api.hpp
    src/boost/locale/win32/collate.cpp
    src/boost/locale/win32/converter.cpp
    src/boost/locale/win32/numeric.cpp
    src/boost/locale/win32/win_backend.cpp
    src/boost/locale/win32/win_backend.hpp
  )

else()

  target_compile_definitions(boost_locale PRIVATE BOOST_LOCALE_NO_WINAPI_BACKEND=1)

endif()

if(BOOST_LOCALE_ENABLE_WINAPI OR (BOOST_LOCALE_ENABLE_STD AND WIN32))

  target_sources(boost_locale PRIVATE
    src/boost/locale/win32/lcid.cpp
    src/boost/locale/win32/lcid.hpp
  )

endif()

if(BOOST_LOCALE_ENABLE_POSIX)

  target_sources(boost_locale PRIVATE
    src/boost/locale/posix/all_generator.hpp
    src/boost/locale/posix/codecvt.cpp
    src/boost/locale/posix/codecvt.hpp
    src/boost/locale/posix/collate.cpp
    src/boost/locale/posix/converter.cpp
    src/boost/locale/posix/numeric.cpp
    src/boost/locale/posix/posix_backend.cpp
    src/boost/locale/posix/posix_backend.hpp
  )

else()

  target_compile_definitions(boost_locale PRIVATE BOOST_LOCALE_NO_POSIX_BACKEND=1)

endif()

if(BOOST_LOCALE_ENABLE_WINAPI OR BOOST_LOCALE_ENABLE_STD OR BOOST_LOCALE_ENABLE_POSIX)

  target_sources(boost_locale PRIVATE
    src/boost/locale/util/gregorian.cpp
    src/boost/locale/util/gregorian.hpp
  )

endif()

message(STATUS "Boost.Locale: "
  "iconv ${BOOST_LOCALE_ENABLE_ICONV}, "
  "ICU ${BOOST_LOCALE_ENABLE_ICU}, "
  "POSIX ${BOOST_LOCALE_ENABLE_POSIX}, "
  "std ${BOOST_LOCALE_ENABLE_STD}, "
  "winapi ${BOOST_LOCALE_ENABLE_WINAPI}"
)

if(MSVC)
    set(BOOST_LOCALE_WARNING_OPTIONS /W4)
    if(BOOST_LOCALE_WERROR)
        list(APPEND BOOST_LOCALE_WARNING_OPTIONS /WX)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(BOOST_LOCALE_WARNING_OPTIONS -Wall -Wextra -Wno-error=deprecated-declarations)
    if(BOOST_LOCALE_WERROR)
        list(APPEND BOOST_LOCALE_WARNING_OPTIONS -Werror)
    endif()
elseif(NOT DEFINED BOOST_LOCALE_WARNING_OPTIONS)
    set(BOOST_LOCALE_WARNING_OPTIONS "")
endif()
target_compile_options(boost_locale PRIVATE ${BOOST_LOCALE_WARNING_OPTIONS})

# Testing

if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")

  add_subdirectory(test)
  add_subdirectory(examples)

endif()
