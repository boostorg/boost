// Copyright (c) 2015 Artyom Beilis (Tonkikh)
// Copyright (c) 2019-2021 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/nowide/fstream.hpp>

#include <boost/nowide/cstdio.hpp>
#include "file_test_helpers.hpp"
#include "test.hpp"
#include <string>

namespace nw = boost::nowide;
using namespace boost::nowide::test;

template<typename T>
void test_ctor(const T& filename)
{
    // Create file if not exist
    ensure_not_exists(filename);
    {
        nw::ofstream f(filename);
        TEST(f);
    }
    TEST(file_exists(filename));
    TEST_EQ(read_file(filename), "");

    // Clear file if exists
    create_file(filename, "test");
    // Default
    {
        nw::ofstream f(filename);
        TEST(f);
    }
    TEST_EQ(read_file(filename), "");

    // At end
    {
        nw::ofstream f(filename, std::ios::ate);
        TEST(f);
    }
    TEST_EQ(read_file(filename), "");

    // Binary mode
    {
        nw::ofstream f(filename, std::ios::binary);
        TEST(f);
        TEST(f << "test\r\n");
    }
    TEST_EQ(read_file(filename, data_type::binary), "test\r\n");
}

template<typename T>
void test_open(const T& filename)
{
    // Create file if not exist
    ensure_not_exists(filename);
    {
        nw::ofstream f;
        f.open(filename);
        TEST(f);
    }
    TEST(file_exists(filename));
    TEST_EQ(read_file(filename), "");

    // Clear file if exists
    create_file(filename, "test");
    // Default
    {
        nw::ofstream f;
        f.open(filename);
        TEST(f);
    }
    TEST_EQ(read_file(filename), "");

    // At end
    {
        nw::ofstream f;
        f.open(filename, std::ios::ate);
        TEST(f);
    }
    TEST_EQ(read_file(filename), "");

    // Binary mode
    {
        nw::ofstream f;
        f.open(filename, std::ios::binary);
        TEST(f);
        TEST(f << "test\r\n");
    }
    TEST_EQ(read_file(filename, data_type::binary), "test\r\n");
}

void test_move_and_swap(const std::string& filename)
{
    const std::string filename2 = filename + ".2";
    remove_file_at_exit _(filename);
    remove_file_at_exit _2(filename2);

    // Move construct
    {
        nw::ofstream f_old(filename);
        TEST(f_old << "Hello ");

        nw::ofstream f_new(std::move(f_old));
        // old is closed
        TEST(!f_old.is_open());
        // It is unclear if the std streams can be reused after move-from
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
        f_old.open(filename2);
#else
        f_old = nw::ofstream(filename2);
#endif
        TEST(f_old << "Foo");

        // new starts where the old was left of
        TEST(f_new);
        TEST(f_new << "World");
    }
    TEST_EQ(read_file(filename), "Hello World");
    TEST_EQ(read_file(filename2), "Foo");

    // Move assign
    {
        nw::ofstream f_new(filename2);
        TEST(f_new << "DiscardThis");
        {
            nw::ofstream f_old(filename);
            TEST(f_old << "Hello ");

            f_new = std::move(f_old);
            // old is closed
            TEST(!f_old.is_open());
            // It is unclear if the std streams can be reused after move-from
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
            f_old.open(filename2);
#else
            f_old = nw::ofstream(filename2);
#endif
            TEST(f_old << "Foo");
        }
        // new starts where the old was left of
        TEST(f_new);
        TEST(f_new << "World");
    }
    TEST_EQ(read_file(filename), "Hello World");
    TEST_EQ(read_file(filename2), "Foo");

    // Swap
    {
        nw::ofstream f_old(filename);
        TEST(f_old << "Hello ");

        nw::ofstream f_new(filename2);
        TEST(f_new << "Foo ");

        // After swapping both are valid and where they left
        f_new.swap(f_old);
        TEST(f_old << "Bar");
        TEST(f_new << "World");

        f_new.close();
        swap(f_new, f_old);
        TEST(!f_old.is_open());
        TEST(f_new.is_open());
    }
    TEST_EQ(read_file(filename), "Hello World");
    TEST_EQ(read_file(filename2), "Foo Bar");
}

// Based on bug reported in #150
void test_reopen(const std::string& filename)
{
    const std::string filename2 = filename + ".2";
    const std::string filename3 = filename + ".3";
    remove_file_at_exit _(filename);
    remove_file_at_exit _2(filename2);
    remove_file_at_exit _3(filename3);

    nw::ofstream f(filename, std::ios_base::binary);
    using nw::test::data_type;
    // Data sizes were randomly selected but above the usual default buffer size of 512
    const std::string testData = nw::test::create_random_data(613, data_type::binary);
    TEST(f.write(testData.c_str(), testData.size()));
    f.close();
    TEST_EQ(read_file(filename, data_type::binary), testData);

    // Reopen via open-function
    f.open(filename2, std::ios_base::binary);
    const std::string testData2 = nw::test::create_random_data(523, data_type::binary);
    TEST(f.write(testData2.c_str(), testData2.size()));
    f.close();
    TEST_EQ(read_file(filename2, data_type::binary), testData2);

    // Reopen via move-assign
    f = nw::ofstream(filename3, std::ios_base::binary);
    const std::string testData3 = nw::test::create_random_data(795, data_type::binary);
    TEST(f.write(testData3.c_str(), testData3.size()));
    f.close();
    TEST_EQ(read_file(filename3, data_type::binary), testData3);
}

// coverity[root_function]
void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_ctor(exampleFilename.c_str());
    test_ctor(exampleFilename);
    test_open(exampleFilename.c_str());
    test_open(exampleFilename);
    test_move_and_swap(exampleFilename);
    test_reopen(exampleFilename);
}
