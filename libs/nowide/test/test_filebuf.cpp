// Copyright (c) 2015 Artyom Beilis (Tonkikh)
// Copyright (c) 2019-2021 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/nowide/filebuf.hpp>

#include "file_test_helpers.hpp"
#include "test.hpp"
#include <algorithm>
#include <cstdint>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

namespace nw = boost::nowide;
using namespace boost::nowide::test;

// Check member types
static_assert(std::is_same<nw::filebuf::char_type, char>::value, "!");
static_assert(std::is_same<nw::filebuf::traits_type::char_type, char>::value, "!");
static_assert(std::is_same<nw::filebuf::int_type, nw::filebuf::traits_type::int_type>::value, "!");
static_assert(std::is_same<nw::filebuf::pos_type, nw::filebuf::traits_type::pos_type>::value, "!");
static_assert(std::is_same<nw::filebuf::off_type, nw::filebuf::traits_type::off_type>::value, "!");

using CharTraits = nw::filebuf::traits_type;
const auto eof = CharTraits::eof();

constexpr std::ios_base::openmode make_mode(std::ios_base::openmode flags, bool binary)
{
    return binary ? flags | std::ios_base::binary : flags;
}

template<class T_FileBuf>
bool open_with_buffer(T_FileBuf& filebuf,
                      const std::string& filepath,
                      std::ios_base::openmode flags,
                      char* buffer,
                      size_t size)
{
    const bool bufferSet = filebuf.pubsetbuf(buffer, size) != nullptr;
    return filebuf.open(filepath, flags) && (bufferSet || filebuf.pubsetbuf(buffer, size));
}

template<class T_FileBuf, size_t T_size>
bool open_with_buffer(T_FileBuf& filebuf,
                      const std::string& filepath,
                      std::ios_base::openmode flags,
                      char (&buffer)[T_size])
{
    return open_with_buffer(filebuf, filepath, flags, buffer, T_size);
}

template<class T_FileBuf>
bool open_unbuffered(T_FileBuf& filebuf, const std::string& filepath, std::ios_base::openmode flags)
{
    return open_with_buffer(filebuf, filepath, flags, nullptr, 0);
}

// Read the given number of chars from the buffer, checking that there are that many
template<class T_Buf>
bool skip_chars(T_Buf& buf, size_t num_chars)
{
    for(size_t i = 0; i < num_chars; ++i)
    {
        if(buf.sbumpc() == eof)
            return false; // LCOV_EXCL_LINE
    }
    return true;
}

void test_open_close(const std::string& filepath)
{
    const std::string filepath2 = filepath + ".2";
    ensure_not_exists(filepath2);
    remove_file_at_exit _(filepath);
    remove_file_at_exit _2(filepath2);

    {
        nw::filebuf buf;
        TEST(buf.open(filepath, std::ios_base::out) == &buf);
        TEST(buf.is_open());

        // Opening when already open fails
        TEST(buf.open(filepath2, std::ios_base::out) == nullptr);
        // Still open
        TEST(buf.is_open());
        TEST(buf.close() == &buf);
        // Failed opening did not create file
        TEST(!file_exists(filepath2));

        // But it should work now:
        TEST(buf.open(filepath2, std::ios_base::out) == &buf);
        TEST(buf.close() == &buf);
        TEST(file_exists(filepath2));
    }

    const auto file_data = create_random_data(20, data_type::text);
    create_file(filepath, file_data);
    // Can't write to read-only buf
    {
        nw::filebuf buf;
        TEST(buf.open(filepath, std::ios_base::in));
        TEST_EQ(buf.sputc('a'), eof);
        // Even if chars were copied to put area, they cannot be written (in sync)
        TEST(buf.sputn("hello", 5) == 0 || buf.pubsync() == -1);
    }
    TEST_EQ(read_file(filepath), file_data); // File is unchanged
    {
        nw::filebuf buf;
        char buffer[3];
        TEST(open_with_buffer(buf, filepath, std::ios_base::in, buffer));
        // Also doesn't write when using direct I/O due to data>buffersize (at least in Nowide)
        TEST(buf.sputn("hello", 5) == 0 || buf.pubsync() == -1);
    }
    TEST_EQ(read_file(filepath), file_data); // File is unchanged

    // Can't read from write-only buf
    {
        for(const auto flags : {std::ios_base::out, std::ios_base::app})
        {
            create_file(filepath, file_data);
            nw::filebuf buf;
            TEST(buf.open(filepath, flags));
            TEST_EQ(buf.sgetc(), eof);
            TEST_EQ(buf.sbumpc(), eof);
            char str[3];
            TEST_EQ(buf.sgetn(str, sizeof(str)), 0);
            // Putback is also just for reading
            TEST(buf.pubseekoff(0, std::ios_base::end) != std::streampos(-1));
            TEST_EQ(buf.sungetc(), eof);
            TEST_EQ(buf.sputbackc('t'), eof);
        }
    }
}

void test_pubseekpos(const std::string& filepath)
{
    const std::string data = create_random_data(BUFSIZ * 4, data_type::binary);
    create_file(filepath, data, data_type::binary);
    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::in | std::ios_base::binary) == &buf);

    // Fuzzy test: Seek to a couple random positions
    std::minstd_rand rng(std::random_device{}());
    using pos_type = nw::filebuf::pos_type;
    const auto eofPos = pos_type(data.size());
    std::uniform_int_distribution<size_t> distr(0, static_cast<size_t>(eofPos) - 1);

    const auto getData = [&](pos_type pos) { return CharTraits::to_int_type(data[static_cast<size_t>(pos)]); };

    for(int i = 0; i < 100; i++)
    {
        const pos_type pos = distr(rng);
        TEST_EQ(buf.pubseekpos(pos), pos);
        TEST_EQ(buf.sgetc(), getData(pos));
    }
    // Seek to first and last as corner case tests
    TEST_EQ(buf.pubseekpos(0), pos_type(0));
    TEST_EQ(buf.sgetc(), CharTraits::to_int_type(data[0]));
    TEST_EQ(buf.pubseekpos(eofPos), eofPos);
    TEST_EQ(buf.sgetc(), eof);
}

void test_pubseekoff(const std::string& filepath)
{
    const std::string data = create_random_data(BUFSIZ * 4, data_type::binary);
    create_file(filepath, data, data_type::binary);
    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::in | std::ios_base::binary) == &buf);

    // Fuzzy test: Seek to a couple random positions
    std::minstd_rand rng(std::random_device{}());
    using pos_type = nw::filebuf::pos_type;
    using off_type = nw::filebuf::off_type;
    const auto eofPos = pos_type(data.size());
    std::uniform_int_distribution<size_t> distr(0, static_cast<size_t>(eofPos) - 1);

    const auto getData = [&](pos_type pos) { return CharTraits::to_int_type(data[static_cast<size_t>(pos)]); };
    // tellg/tellp function as called by basic_[io]fstream
    const auto tellg = [&]() { return buf.pubseekoff(0, std::ios_base::cur); };

    for(int i = 0; i < 100; i++)
    {
        // beg
        pos_type pos = distr(rng);
        TEST_EQ(buf.pubseekoff(pos, std::ios_base::beg), pos);
        TEST_EQ(tellg(), pos);
        TEST_EQ(buf.sgetc(), getData(pos));
        // cur
        off_type diff = static_cast<pos_type>(distr(rng)) - pos;
        pos += diff;
        TEST_EQ(buf.pubseekoff(diff, std::ios_base::cur), pos);
        TEST_EQ(tellg(), pos);
        TEST_EQ(buf.sgetc(), getData(pos));
        // end
        diff = static_cast<pos_type>(distr(rng)) - eofPos;
        pos = eofPos + diff;
        TEST_EQ(buf.pubseekoff(diff, std::ios_base::end), pos);
        TEST_EQ(tellg(), pos);
        TEST_EQ(buf.sgetc(), getData(pos));
    }
    // Seek to first and last as corner case tests
    TEST_EQ(buf.pubseekoff(0, std::ios_base::beg), pos_type(0));
    TEST_EQ(tellg(), pos_type(0));
    TEST_EQ(buf.sgetc(), CharTraits::to_int_type(data[0]));
    TEST_EQ(buf.pubseekoff(0, std::ios_base::end), eofPos);
    TEST_EQ(tellg(), eofPos);
    TEST_EQ(buf.sgetc(), eof);
}

void test_64_bit_seek(const std::string& filepath)
{
    // Create a value which does not fit into a 32 bit value.
    // Use an unsigned intermediate to have the truncation defined to wrap to 0
    using unsigned_off_type = std::make_unsigned<nw::filebuf::off_type>::type;
    nw::filebuf::off_type offset = static_cast<unsigned_off_type>(std::uint64_t(1) << 33u);

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4127)
#endif
    // if we can't use 64 bit offsets through the API, don't test anything
    // coverity[result_independent_of_operands]
    if(offset == nw::filebuf::off_type(0))
    {
        // coverity[dead_error_line]
        return; // LCOV_EXCL_LINE
    }
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

    create_file(filepath, "test");
    remove_file_at_exit _(filepath);

    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::in | std::ios_base::binary) == &buf);
    const std::streampos knownPos = 2;
    TEST_EQ(buf.pubseekpos(knownPos), knownPos); // Just to make sure we know where we are
    const std::streampos newPos = buf.pubseekoff(offset, std::ios_base::cur);
    // On 32 bit mode or when seek beyond EOF is not allowed, the current position should be unchanged
    if(newPos == std::streampos(-1))
        TEST_EQ(buf.pubseekoff(0, std::ios_base::cur), knownPos);
    else
    {
#if !BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
        // libc++ may truncate the 64 bit value when calling fseek which yields an offset of 0
        if(newPos == knownPos)
            offset = 0; // LCOV_EXCL_LINE
#endif
        TEST_EQ(newPos, offset + knownPos);
        TEST_EQ(buf.pubseekoff(0, std::ios_base::cur), newPos);
    }
}

void test_xsgetn(const std::string& filepath, bool binary)
{
    char buffer[200]{};
    const auto dataType = binary ? data_type::binary : data_type::text;
    const std::string data = create_random_data(sizeof(buffer) + 50, dataType);
    create_file(filepath, data, dataType);

    for(const bool unbuffered : {false, true})
    {
        TEST_CONTEXT((unbuffered ? "unbuffered" : "buffered"));
        nw::filebuf buf;
        if(unbuffered)
            TEST(open_unbuffered(buf, filepath, make_mode(std::ios_base::in, binary)));
        else
            TEST(open_with_buffer(buf, filepath, make_mode(std::ios_base::in, binary), buffer));

        std::string strBuf(data.size() * 2, '\0');
        // Reading stops at EOF
        TEST_EQ(buf.sgetn(&strBuf[0], strBuf.size()), static_cast<std::streamsize>(data.size()));
        strBuf.resize(data.size());
        TEST_EQ(strBuf, data);
        TEST(buf.pubseekpos(0) != std::streampos(-1));
        // Read a bit using regular underflow, then via sgetn and again via underflow
        TEST_EQ(buf.sbumpc(), CharTraits::to_int_type(data[0]));
        TEST_EQ(buf.sbumpc(), CharTraits::to_int_type(data[1]));
        strBuf.clear();
        // Go definitely over a buffer boundary
        strBuf.resize(sizeof(buffer));
        TEST_EQ(buf.sgetn(&strBuf[0], strBuf.size()), static_cast<std::streamsize>(strBuf.size()));
        TEST_EQ(strBuf, data.substr(2, strBuf.size()));
        TEST_EQ(buf.sbumpc(), CharTraits::to_int_type(data[strBuf.size() + 2]));
        TEST_EQ(buf.sbumpc(), CharTraits::to_int_type(data[strBuf.size() + 3]));
    }
    // Corner cases:
    //   - sgetn with zero or negative count is a no-op
    //   - sgetn fails on closed filebuf
    for(const bool unbuffered : {false, true})
    {
        TEST_CONTEXT((unbuffered ? "unbuffered" : "buffered"));
        create_file(filepath, "Hello World");
        nw::filebuf buf;
        // Set a buffer just to see if it is written to
        if(unbuffered)
            TEST(open_unbuffered(buf, filepath, make_mode(std::ios_base::in, binary)));
        else
            TEST(open_with_buffer(buf, filepath, make_mode(std::ios_base::in, binary), buffer));

        std::string str = create_random_data(data.size(), data_type::binary);
        const auto origStr = str;
        buffer[0] = origStr[0];

        TEST_EQ(buf.sgetn(&str[0], 0), 0);
#if defined(__GNUC__) && __GNUC__ >= 7
        // GCC may not detect that the negative value is checked by xsgetn
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wrestrict"
#endif
        // coverity[negative_returns]
        TEST_EQ(buf.sgetn(&str[0], -1), 0);
        // coverity[negative_returns]
        TEST_EQ(buf.sgetn(&str[0], -999), 0);
#if defined(__GNUC__) && __GNUC__ >= 12
#pragma GCC diagnostic pop
#endif
        buf.close();
        // No read when closed
        TEST_EQ(buf.sgetn(&str[0], 1), 0);
        TEST_EQ(str, origStr);
        TEST_EQ(buffer[0], origStr[0]);
        TEST_EQ(buf.sgetn(&str[0], str.size()), 0);
        TEST_EQ(str, origStr);
        TEST_EQ(buffer[0], origStr[0]);
    }
}

void test_xsputn(const std::string& filepath, bool binary)
{
    char buffer[200]{};
    const auto dataType = binary ? data_type::binary : data_type::text;
    const std::string data = create_random_data(sizeof(buffer) + 50, dataType);

    for(const bool unbuffered : {false, true})
    {
        TEST_CONTEXT((unbuffered ? "unbuffered" : "buffered"));
        nw::filebuf buf;
        const auto flags = make_mode(std::ios_base::out | std::ios_base::trunc, binary);
        if(unbuffered)
            TEST(open_unbuffered(buf, filepath, flags));
        else
            TEST(open_with_buffer(buf, filepath, flags, buffer));

        TEST_EQ(buf.sputn(data.data(), data.size()), static_cast<std::streamsize>(data.size()));
        buf.close();
        TEST_EQ(read_file(filepath, dataType), data);

        // Write a bit using regular overflow, then via sputn and back using overflow
        TEST(buf.open(filepath, flags));
        TEST_EQ(buf.sputc(data[0]), CharTraits::to_int_type(data[0]));
        TEST_EQ(buf.sputc(data[1]), CharTraits::to_int_type(data[1]));
        // This is more than 1 buffer size
        std::streamsize numBytesToWrite = data.size() - 4;
        TEST_EQ(buf.sputn(&data[2], numBytesToWrite), numBytesToWrite);
        TEST_EQ(buf.sputc(data[data.size() - 2]), CharTraits::to_int_type(data[data.size() - 2]));
        TEST_EQ(buf.sputc(data[data.size() - 1]), CharTraits::to_int_type(data[data.size() - 1]));
        buf.close();
        TEST_EQ(read_file(filepath, dataType), data);
    }
    // Corner cases:
    //   - sputn with zero or negative count is a no-op
    //   - sputn fails on closed filebuf
    for(const bool unbuffered : {false, true})
    {
        TEST_CONTEXT((unbuffered ? "unbuffered" : "buffered"));
        nw::filebuf buf;
        if(unbuffered)
            TEST(open_unbuffered(buf, filepath, make_mode(std::ios_base::out | std::ios_base::trunc, binary)));
        else
            TEST(open_with_buffer(buf, filepath, make_mode(std::ios_base::out | std::ios_base::trunc, binary), buffer));

        TEST_EQ(buf.sputn(data.data(), 0), 0);
        // coverity[negative_returns]
        TEST_EQ(buf.sputn(data.data(), -1), 0);
        // coverity[negative_returns]
        TEST_EQ(buf.sputn(data.data(), -999), 0);
        buf.close();
        // No write when closed
        TEST_EQ(buf.sputn(data.data(), 1), 0);
        TEST_EQ(buf.sputn(data.data(), data.size()), 0);
        TEST_EQ(read_file(filepath), "");
    }
}

void test_read_write_switch(const std::string& filepath, bool binary)
{
    // Switching between read and write requires a seek or (for W->R) a sync
    const std::string data = "1234567890";
    nw::filebuf buf;
    TEST(buf.open(filepath, make_mode(std::ios_base::in | std::ios_base::out | std::ios_base::trunc, binary)));
    TEST_EQ(buf.sputn(data.data(), data.size()), static_cast<std::streamsize>(data.size()));
    // W->R via seek
    buf.pubseekpos(0);
    TEST_EQ(buf.sbumpc(), '1');
    // R->W via seek
    const auto pos = buf.pubseekoff(0, std::ios_base::cur);
    TEST(pos != std::streampos(-1));
    buf.sputc('b');
    // W->R via sync
    TEST(buf.pubsync() == 0);
    TEST_EQ(buf.sbumpc(), '3');
    // R->W via seek
    const auto pos2 = buf.pubseekoff(0, std::ios_base::cur);
    buf.sputc('c');
    // Read right back
    TEST_EQ(buf.pubseekpos(pos2), pos2);
    TEST_EQ(buf.sbumpc(), 'c');
    // R->W
    buf.pubseekoff(0, std::ios_base::cur);
    buf.sputc('d');
    // Sync & seek
    TEST(buf.pubsync() == 0);
    TEST(buf.pubseekoff(0, std::ios_base::cur) != std::streampos(-1));
    TEST_EQ(buf.sbumpc(), '6');
    // R->W
    buf.pubseekoff(0, std::ios_base::cur);
    buf.sputc('e');
    // Seek & sync
    TEST(buf.pubseekoff(0, std::ios_base::cur) != std::streampos(-1));
    TEST(buf.pubsync() == 0);
    TEST_EQ(buf.sbumpc(), '8');

    buf.close();
    TEST_EQ(read_file(filepath), "1b3cd6e890");
}

void subtest_sync(const std::string& filepath, bool binary, const std::string& data)
{
    nw::filebuf buf;
    // Use a small buffer to force filling it up w/o requiring to write lot's of data
    char buffer[3];
    buf.pubsetbuf(buffer, sizeof(buffer));
    const auto flags = make_mode(std::ios_base::out | std::ios_base::trunc, binary);

    // Do a series of single-char and multi-char writes with varying size combinations
    // Especially test the case of only single-char and only multi-char ops
    for(unsigned singleCharOps = 0; singleCharOps <= 3; ++singleCharOps)
    {
        // Write less than buffer size, 1 or 2 buffers or even more, assuming buffer size of 3
        for(size_t bufSize : {0, 2, 3, 6, 7})
        {
            if(singleCharOps + bufSize == 0u)
                continue;
            TEST(buf.open(filepath, flags));
            for(size_t i = 0; i < data.size();)
            {
                TEST_CONTEXT("sc:" << singleCharOps << " buf:" << bufSize << " i:" << i);
                for(unsigned j = 0; j < singleCharOps && i < data.size(); ++j, ++i)
                {
                    TEST_EQ(buf.sputc(data[i]), CharTraits::to_int_type(data[i]));
                }
                if(bufSize != 0u)
                {
                    const auto remainSize = std::min(data.size() - i, bufSize);
                    TEST_EQ(buf.sputn(&data[i], remainSize), static_cast<std::streamsize>(remainSize));
                    i += remainSize;
                }
                TEST_EQ(buf.pubsync(), 0);
                TEST_EQ(read_file(filepath, binary ? data_type::binary : data_type::text), data.substr(0, i));
            }
            TEST(buf.close());
            TEST_EQ(read_file(filepath, binary ? data_type::binary : data_type::text), data);
        }
    }
}

void subtest_singlechar_positioning(const std::string& filepath, bool binary, const std::string& data)
{
    nw::filebuf buf;
    // Use a small buffer to force filling it up w/o requiring to write lot's of data
    char buffer[3]{};
    const auto mode = make_mode(std::ios_base::in | std::ios_base::out | std::ios_base::trunc, binary);
    TEST(open_with_buffer(buf, filepath, mode, buffer));

    // Put each char and record its position
    std::vector<nw::filebuf::pos_type> pos(data.size());
    for(unsigned i = 0; i < data.size(); ++i)
    {
        buf.sputc(data[i]);
        pos[i] = buf.pubseekoff(0, std::ios_base::cur);
    }
    // Go back to start and verify reading yields the same data and positions
    buf.pubseekoff(0, std::ios_base::beg);
    for(unsigned i = 0; i < data.size(); ++i)
    {
        TEST_CONTEXT("Position " << i);
        TEST_EQ(buf.sbumpc(), CharTraits::to_int_type(data[i]));
        TEST_EQ(buf.pubseekoff(0, std::ios_base::cur), pos[i]);
    }
}

void subtest_singlechar_multichar_reads(const std::string& filepath, bool binary, const std::string& data)
{
    create_file(filepath, data, binary ? data_type::binary : data_type::text);
    nw::filebuf buf;
    // Use a small buffer to force filling it up w/o requiring to write lot's of data
    char buffer[3]{};
    TEST(open_with_buffer(buf, filepath, make_mode(std::ios_base::in, binary), buffer));

    // Do a series of single-char and multi-char reads with varying size combinations
    // Especially test the case of only single-char and only multi-char ops
    for(unsigned singleCharOps = 0; singleCharOps <= 3; ++singleCharOps)
    {
        // Read less than buffer size, 1 or 2 buffers or even more, assuming buffer size of 3
        for(size_t bufSize : {0, 2, 3, 6, 7})
        {
            if(singleCharOps + bufSize == 0u)
                continue;

            std::string outBuf(bufSize, '\0');
            buf.pubseekoff(0, std::ios_base::beg);
            for(size_t i = 0; i < data.size();)
            {
                TEST_CONTEXT("sc:" << singleCharOps << " buf:" << bufSize << " i:" << i);
                for(unsigned j = 0; j < singleCharOps && i < data.size(); ++j, ++i)
                {
                    TEST_EQ(buf.sbumpc(), CharTraits::to_int_type(data[i]));
                }
                if(bufSize == 0u)
                    continue;
                const size_t readSize = std::min(data.size() - i, bufSize);
                TEST_EQ(buf.sgetn(&outBuf.front(), bufSize), static_cast<std::streamsize>(readSize));
                if(readSize < bufSize)
                    outBuf.resize(readSize);
                TEST_EQ(outBuf, data.substr(i, readSize));
                i += bufSize;
            }
        }
    }
}

void test_sungetc(const std::string& filepath, bool binary)
{
    const std::string data = "012345\n6";
    create_file(filepath, data, binary ? data_type::binary : data_type::text);

    // Use a small buffer to force filling it up w/o requiring to write lot's of data
    char buffer[4];
    nw::filebuf buf;
    TEST(open_with_buffer(buf, filepath, make_mode(std::ios_base::in, binary), buffer));

    // Nothing to unget at beginning of file
    TEST_EQ(buf.sungetc(), eof);

    // Able to unget first char and get it again
    TEST_EQ(buf.sbumpc(), '0');
    TEST(buf.sungetc() != eof);
    TEST_EQ(buf.sbumpc(), '0');

    // Able to unget and reread after filling up new buffer
    TEST(skip_chars(buf, sizeof(buffer) - 1u)); // skip remaining chars
    TEST_EQ(buf.sbumpc(), '4');
    TEST(buf.sungetc() != eof);
    // Ungetting multiple chars may or may not be possible
    if(buf.sungetc() != eof)
        TEST_EQ(buf.sbumpc(), '3');
    TEST_EQ(buf.sbumpc(), '4');

    // \n also works
    TEST_EQ(buf.sbumpc(), '5');
    TEST_EQ(buf.sbumpc(), '\n');
    TEST(buf.sungetc() != eof);
    TEST_EQ(buf.sbumpc(), '\n');
    TEST_EQ(buf.sbumpc(), '6');
    TEST(buf.sungetc() != eof);
    if(buf.sungetc() != eof)
        TEST_EQ(buf.sbumpc(), '\n');
    TEST_EQ(buf.sbumpc(), '6');

    // Go back as far as possible
    auto idx = data.size();
    while(buf.sungetc() != eof)
    {
        TEST(idx > 0u);
        --idx;
    }
    TEST(idx < data.size()); // At least 1
    // Get all put back chars
    for(; idx < data.size(); ++idx)
        TEST_EQ(buf.sbumpc(), data[idx]);
}

void test_sputbackc(const std::string& filepath, bool binary)
{
    const std::string data = "012345\n6";
    create_file(filepath, data, binary ? data_type::binary : data_type::text);

    // Use a small buffer to force filling it up w/o requiring to write lot's of data
    char buffer[4];
    nw::filebuf buf;
    TEST(open_with_buffer(buf, filepath, make_mode(std::ios_base::in, binary), buffer));
    TEST(skip_chars(buf, data.size()));

    // Put back same chars explicitly (as many as possible)
    auto idx = data.size();
    while(true)
    {
        auto res = buf.sputbackc((idx > 0u) ? data[idx - 1] : 'z');
        if(res == eof)
            break;
        TEST(idx > 0u);
        TEST_EQ(res, data[idx - 1]);
        --idx;
    }
    TEST(idx < data.size()); // At least 1
    // Get all put back chars
    for(; idx < data.size(); ++idx)
        TEST_EQ(buf.sbumpc(), data[idx]);

    // Put back different chars (as many as possible)
    const std::string data2 = "789\nab\nc";
    TEST_EQ(data.size(), data2.size());
    idx = data2.size();
    while(true)
    {
        auto res = buf.sputbackc((idx > 0u) ? data2[idx - 1] : 'z');
        if(res == eof)
            break;
        TEST(idx > 0u);
        TEST_EQ(res, data2[idx - 1]);
        --idx;
#if !BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
        break; // Some stdlibs fail on putting back multiple different chars
#endif
    }
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
    // At least 1 when using custom filebuf
    // But e.g. libc++ doesn't allow putting back a different char
    TEST(idx < data2.size());
#endif
    // Get all put back chars
    for(; idx < data2.size(); ++idx)
        TEST_EQ(buf.sbumpc(), data2[idx]);
}

void test_textmode(const std::string& filepath)
{
    // Test input, output and getting the file position works for text files with newlines
    const std::string data = []() {
        // Some simple test data
        std::string result = "1234567890";
        // Line break after every char
        result.reserve(result.size() + 27 * 2);
        for(char c = 'a'; c <= 'z'; ++c)
            (result += c) += '\n';
        // Some continuous line breaks
        result.append(4, '\n');
        return result;
    }();
    subtest_singlechar_positioning(filepath, false, data);
    subtest_singlechar_multichar_reads(filepath, false, data);
    subtest_sync(filepath, false, data);
}

// Almost the same test as test_textmode but uses a binary stream.
// Useful as the buffer handling is very different
void test_binarymode(const std::string& filepath)
{
    const std::string data = "123" + create_random_data(65, data_type::binary);
    subtest_singlechar_positioning(filepath, true, data);
    subtest_singlechar_multichar_reads(filepath, true, data);
    subtest_sync(filepath, true, data);
}

void test_swap(const std::string& filepath)
{
    const std::string filepath2 = filepath + ".2";
    remove_file_at_exit _(filepath);
    remove_file_at_exit _2(filepath2);

    // Note: Make sure to have en uneven number of swaps so the destructor runs on the others data

    // Check: FILE*, buffer, buffer_size
    {
        nw::filebuf buf1, buf2;
        char buffer1[3]{}, buffer2[5]{};
        buf1.pubsetbuf(buffer1, sizeof(buffer1));
        buf2.pubsetbuf(buffer2, sizeof(buffer2));
        TEST(buf1.open(filepath, std::ios_base::out) == &buf1);
        buf1.swap(buf2);
        TEST(!buf1.is_open());
        TEST(buf2.is_open());
        TEST(buf1.open(filepath2, std::ios_base::out | std::ios_base::binary) == &buf1);

        // Write "FooBar" to filepath and "HelloWorld" to filepath2
        buf1.sputc('H');
        buf1.sputn("ello", 4);
        buf2.sputc('F');
        buf2.sputn("oo", 2);
        buf2.swap(buf1);
        buf1.sputc('B');
        buf1.sputn("ar", 2);
        buf2.sputc('W');
        buf2.sputn("orld", 4);

        buf1.close();
        TEST(!buf1.is_open());
        TEST(buf2.is_open());
        buf1.swap(buf2);
        TEST(buf1.is_open());
        TEST(!buf2.is_open());
        buf1.close();
        TEST(!buf1.is_open());
        TEST(!buf2.is_open());
        TEST_EQ(read_file(filepath), "FooBar");
        TEST_EQ(read_file(filepath2), "HelloWorld");
    }
    // Check: mode, owns_buffer
    {
        nw::filebuf buf1, buf2;
        char buffer[3]{};
        buf1.pubsetbuf(buffer, sizeof(buffer));
        TEST(buf1.open(filepath, std::ios_base::out) == &buf1);
        TEST(buf2.open(filepath2, std::ios_base::in) == &buf2);
        TEST_EQ(buf1.sputc('B'), 'B');
        TEST_EQ(buf2.sbumpc(), 'H');
        buf1.swap(buf2);
        // Trying to read in write mode or other way round should fail
        TEST_EQ(buf1.sputc('x'), eof);
        TEST_EQ(buf2.sbumpc(), eof);
        TEST_EQ(buf1.sbumpc(), 'e');
        TEST_EQ(buf2.sputc('a'), 'a');
        buf2.swap(buf1);
        TEST_EQ(buf2.sputc('x'), eof);
        TEST_EQ(buf1.sbumpc(), eof);
        TEST_EQ(buf2.sbumpc(), 'l');
        TEST_EQ(buf1.sputn("zXYZ", 4), 4);
        swap(buf2, buf1);
        buf1.close();
        buf2.close();
        TEST_EQ(read_file(filepath), "BazXYZ");
        TEST_EQ(read_file(filepath2), "HelloWorld");
    }
    // Check: last_char, gptr, eback
    {
        nw::filebuf buf1, buf2;
        // Need to disable buffering to use last_char, but only for 1 to detect wrong conditions
        buf1.pubsetbuf(0, 0);
        TEST(buf1.open(filepath, std::ios_base::in) == &buf1);
        TEST(buf2.open(filepath2, std::ios_base::in) == &buf2);
        // Peek
        TEST_EQ(buf1.sgetc(), 'B');
        TEST_EQ(buf2.sgetc(), 'H');
        swap(buf1, buf2);
        TEST_EQ(buf2.sgetc(), 'B');
        TEST_EQ(buf1.sgetc(), 'H');
        // Advance
        TEST_EQ(buf2.sbumpc(), 'B');
        TEST_EQ(buf1.sbumpc(), 'H');
        TEST_EQ(buf2.sbumpc(), 'a');
        TEST_EQ(buf1.sbumpc(), 'e');
        swap(buf1, buf2);
        TEST_EQ(buf1.sbumpc(), 'z');
        TEST_EQ(buf2.sbumpc(), 'l');
        swap(buf1, buf2);
        TEST_EQ(buf2.sgetc(), 'X');
        TEST_EQ(buf1.sgetc(), 'l');
    }
    // Check: pptr, epptr
    {
        nw::filebuf buf1, buf2;
        // Need to disable buffering to use last_char, but only for 1 to detect wrong conditions
        buf1.pubsetbuf(0, 0);
        TEST(buf1.open(filepath, std::ios_base::out) == &buf1);
        TEST(buf2.open(filepath2, std::ios_base::out) == &buf2);
        TEST_EQ(buf1.sputc('1'), '1');
        TEST_EQ(buf2.sputc('a'), 'a');
        swap(buf1, buf2);
        // buf1: filepath2, buf2: filepath
        TEST_EQ(buf1.sputc('b'), 'b');
        TEST_EQ(buf2.sputc('2'), '2');
        // Sync and check if file was written
        TEST_EQ(buf1.pubsync(), 0);
        TEST_EQ(read_file(filepath2), "ab");
        TEST_EQ(buf2.pubsync(), 0);
        TEST_EQ(read_file(filepath), "12");
        swap(buf1, buf2);
        // buf1: filepath, buf2: filepath2
        TEST_EQ(buf1.pubsync(), 0);
        TEST_EQ(read_file(filepath), "12");
        TEST_EQ(buf2.pubsync(), 0);
        TEST_EQ(read_file(filepath2), "ab");
        TEST_EQ(buf1.sputc('3'), '3');
        TEST_EQ(buf2.sputc('c'), 'c');
        swap(buf1, buf2);
        // buf1: filepath2, buf2: filepath
        TEST_EQ(buf1.pubsync(), 0);
        TEST_EQ(read_file(filepath2), "abc");
        TEST_EQ(buf2.pubsync(), 0);
        TEST_EQ(read_file(filepath), "123");
    }
}

// coverity[root_function]
void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";
    test_open_close(exampleFilename);
    test_pubseekpos(exampleFilename);
    test_pubseekoff(exampleFilename);
    test_64_bit_seek(exampleFilename);
    for(const auto binary : {false, true})
    {
        std::cout << "Testing " << (binary ? "binary" : "text") << " mode\n";
        remove_file_at_exit _(exampleFilename);
        test_xsgetn(exampleFilename, binary);
        test_xsputn(exampleFilename, binary);
        test_read_write_switch(exampleFilename, binary);
        test_sungetc(exampleFilename, binary);
        test_sputbackc(exampleFilename, binary);
        binary ? test_binarymode(exampleFilename) : test_textmode(exampleFilename);
    }
// These tests are only useful for the nowide filebuf and are known to fail for
// std::filebuf due to bugs in libc++
#if BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
    test_swap(exampleFilename);
#endif
}
