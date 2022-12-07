//
// Copyright (c) 2020 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/nowide/detail/is_path.hpp>
#include <boost/nowide/detail/is_string_container.hpp>
#include <boost/nowide/fstream.hpp>
#include "test.hpp"
#include <iostream>
#include <string>

#ifdef __has_include
#if __has_include(<version>)
#include <version>
#endif
#endif

#ifdef __cpp_lib_string_view
#include <string_view>
#define BOOST_NOWIDE_TEST_STD_STRINGVIEW
#endif

// Exclude apple as support there is target level specific -.-
#if defined(__cpp_lib_filesystem) && !defined(__APPLE__)
#include <filesystem>
#define BOOST_NOWIDE_TEST_STD_PATH
#endif
#if defined(__cpp_lib_experimental_filesystem)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#define BOOST_NOWIDE_TEST_STD_EXPERIMENTAL_PATH
#endif

#ifdef BOOST_NOWIDE_TEST_BFS_PATH
#if defined(__GNUC__) && __GNUC__ >= 7
#pragma GCC diagnostic ignored "-Wattributes"
#endif
#if defined(_MSC_VER)
#pragma warning(disable : 4714) // function marked as __forceinline not inlined
#endif
#include <boost/filesystem/path.hpp>
#endif

template<class T_Class, class T_Arg, typename = void>
struct has_open : std::false_type
{};
using boost::nowide::detail::void_t;
template<class T_Class, class T_Arg>
struct has_open<T_Class,
                T_Arg,
                void_t<decltype(std::declval<T_Class>().open(std::declval<T_Arg>(), std::ios_base::openmode{}))>>
    : std::true_type
{};

using boost::nowide::detail::is_string_container;
static_assert(is_string_container<std::string, true>::value, "!");
static_assert(is_string_container<std::wstring, false>::value, "!");
static_assert(is_string_container<std::u16string, false>::value, "!");
static_assert(is_string_container<std::u32string, false>::value, "!");
static_assert(!is_string_container<int, true>::value, "!");
static_assert(!is_string_container<int, false>::value, "!");

using boost::nowide::detail::get_data_width;
static_assert(get_data_width<std::string>::value == sizeof(char), "!");
static_assert(get_data_width<std::wstring>::value == sizeof(wchar_t), "!");
static_assert(get_data_width<std::u16string>::value == sizeof(char16_t), "!");
static_assert(get_data_width<std::u32string>::value == sizeof(char32_t), "!");

// coverity[root_function]
void test_main(int, char**, char**)
{
#ifdef BOOST_NOWIDE_TEST_STD_STRINGVIEW
    std::cout << "Testing string_view" << std::endl;
    static_assert(is_string_container<std::string_view, true>::value, "!");
    static_assert(is_string_container<std::wstring_view, false>::value, "!");
    static_assert(is_string_container<std::u16string_view, false>::value, "!");
    static_assert(is_string_container<std::u32string_view, false>::value, "!");
#endif
#ifdef BOOST_NOWIDE_TEST_STD_PATH
    std::cout << "Testing std::filesystem::path" << std::endl;
    using fs_path = std::filesystem::path;
    static_assert(boost::nowide::detail::is_path<fs_path>::value, "!");
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
    static_assert(has_open<boost::nowide::filebuf, fs_path>::value, "!");
#endif
    static_assert(has_open<boost::nowide::ifstream, fs_path>::value, "!");
    static_assert(has_open<boost::nowide::ofstream, fs_path>::value, "!");
    static_assert(has_open<boost::nowide::fstream, fs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ifstream, fs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ofstream, fs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::fstream, fs_path>::value, "!");
    static_assert(has_open<boost::nowide::filebuf, const fs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::ifstream, const fs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::ofstream, const fs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::fstream, const fs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ifstream, const fs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ofstream, const fs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::fstream, const fs_path::value_type*>::value, "!");
#endif
#ifdef BOOST_NOWIDE_TEST_STD_EXPERIMENTAL_PATH
    std::cout << "Testing std::experimental::filesystem::path" << std::endl;
    using exfs_path = std::experimental::filesystem::path;
    static_assert(boost::nowide::detail::is_path<exfs_path>::value, "!");
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
    static_assert(has_open<boost::nowide::filebuf, exfs_path>::value, "!");
#endif
    static_assert(has_open<boost::nowide::ifstream, exfs_path>::value, "!");
    static_assert(has_open<boost::nowide::ofstream, exfs_path>::value, "!");
    static_assert(has_open<boost::nowide::fstream, exfs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ifstream, exfs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ofstream, exfs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::fstream, exfs_path>::value, "!");
    static_assert(has_open<boost::nowide::filebuf, const exfs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::ifstream, const exfs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::ofstream, const exfs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::fstream, const exfs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ifstream, const exfs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ofstream, const exfs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::fstream, const exfs_path::value_type*>::value, "!");
#endif
#ifdef BOOST_NOWIDE_TEST_BFS_PATH
    std::cout << "Testing boost::filesystem::path" << std::endl;
    using bfs_path = boost::filesystem::path;
    static_assert(boost::nowide::detail::is_path<bfs_path>::value, "!");
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
    static_assert(has_open<boost::nowide::filebuf, bfs_path>::value, "!");
#endif
    static_assert(has_open<boost::nowide::ifstream, bfs_path>::value, "!");
    static_assert(has_open<boost::nowide::ofstream, bfs_path>::value, "!");
    static_assert(has_open<boost::nowide::fstream, bfs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ifstream, bfs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ofstream, bfs_path>::value, "!");
    static_assert(std::is_constructible<boost::nowide::fstream, bfs_path>::value, "!");
    static_assert(has_open<boost::nowide::filebuf, const bfs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::ifstream, const bfs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::ofstream, const bfs_path::value_type*>::value, "!");
    static_assert(has_open<boost::nowide::fstream, const bfs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ifstream, const bfs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::ofstream, const bfs_path::value_type*>::value, "!");
    static_assert(std::is_constructible<boost::nowide::fstream, const bfs_path::value_type*>::value, "!");
#endif
}
