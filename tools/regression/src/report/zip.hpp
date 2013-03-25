// zip.hpp
//
// Copyright (c) 2010, 2013
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ZIP_ZIP_HPP_INCLUDED
#define BOOST_ZIP_ZIP_HPP_INCLUDED

#include <cstring>
#include <ostream>
#include <string>
#include <cstddef>
#include <vector>
#include <bitset>
#include <ios>

#include <boost/array.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/cstdint.hpp>
#include <boost/crc.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost {
namespace zip {

// TODO: Handle endian conversions
#define BOOST_ZIP_DEFINE_HEADER(name, type, offset)                 \
    static const int name##_offset = (offset);                      \
    static type get_##name(const char* header) {                    \
        type result;                                                \
        ::std::memcpy(&result, header + (offset), sizeof(type));    \
        return result;                                              \
    }                                                               \
    static void set_##name(char* header, type x) {                  \
        ::std::memcpy(header + (offset), &x, sizeof(type));         \
    }

class zip_archive {
public:
    zip_archive(std::ostream& file) : output_file(file), current_offset(0), num_files(0) {}
    ~zip_archive() {
        close();
    }
    
    class file_handle;
    friend class file_handle;
    class file_handle {
    public:
        typedef char char_type;

        struct category :
            ::boost::iostreams::sink_tag,
            ::boost::iostreams::closable_tag
        {};

        file_handle(zip_archive& archive,
                    const std::string& path,
                    boost::uint16_t creator_version,
                    boost::uint16_t minimum_required_version,
                    boost::uint16_t flags,
                    boost::uint16_t compression_method,
                    const boost::posix_time::ptime& modification_time) 
        {
            self = 0;
            archive.open_file(path, creator_version, minimum_required_version, flags, compression_method, modification_time, this);
        }

        file_handle(zip_archive& archive,
                    const std::string& path,
                    boost::uint16_t creator_version,
                    boost::uint16_t minimum_required_version,
                    boost::uint16_t flags,
                    boost::uint16_t compression_method) 
        {
            self = 0;
            archive.open_file(path.data(), path.size(), creator_version, minimum_required_version, flags, compression_method, 0, 0, this);
        }
        ::std::streamsize write(const char* data, ::std::streamsize size) {
            assert(self != 0);
            self->output_file.write(data, size);
            compressed_size += size;
            self->current_offset += size;
            return size;
        }
        void write_uncompressed(const char* data, ::std::streamsize size) {
            assert(self != 0);
            crc.process_bytes(data, static_cast<std::size_t>(size));
            uncompressed_size += size;
        }
        void close() {
            central_directory_entry::set_crc32(&self->central_directory[offset], crc.checksum());
            // These lines cause a warning.  Since the warning is legitimate,
            // I'm leaving it.
            central_directory_entry::set_compressed_size(&self->central_directory[offset], compressed_size);
            central_directory_entry::set_uncompressed_size(&self->central_directory[offset], uncompressed_size);

            boost::array<char, 12> buffer;
            data_descriptor::set_crc32(&buffer[0], crc.checksum());
            data_descriptor::set_compressed_size(&buffer[0], compressed_size);
            data_descriptor::set_uncompressed_size(&buffer[0], uncompressed_size);
            std::streamsize current_pos = self->output_file.tellp();
            self->output_file.seekp(pos);
            self->output_file.write(&buffer[0], 12);
            self->output_file.seekp(current_pos);
            self = 0;
        }
    private:
        friend class zip_archive;
        file_handle(const file_handle&);
        file_handle& operator=(const file_handle&);

        boost::crc_32_type crc;
        std::streamsize pos;
        std::size_t offset;
        std::streamsize compressed_size;
        std::streamsize uncompressed_size;
        zip_archive* self;
    };
    
    void open_file(const std::string& path,
                   boost::uint16_t creator_version,
                   boost::uint16_t minimum_required_version,
                   boost::uint16_t flags,
                   boost::uint16_t compression_method,
                   const boost::posix_time::ptime& modification_time,
                   file_handle* out
                   )
    {
        boost::uint16_t date =
            modification_time.date().day() +
            (modification_time.date().month() << 5) +
            ((modification_time.date().year() - 1980) << 9);
        boost::uint16_t time =
            (modification_time.time_of_day().seconds() / 2) +
            (modification_time.time_of_day().minutes() << 5) +
            (modification_time.time_of_day().hours() << 11);
        open_file(path.data(), path.size(), creator_version, minimum_required_version, flags, compression_method, time, date, out);
    }

    void open_file(const char* path, std::size_t path_size,
                   boost::uint16_t creator_version,
                   boost::uint16_t minimum_required_version,
                   boost::uint16_t flags,
                   boost::uint16_t compression_method,
                   boost::uint16_t modification_time,
                   boost::uint16_t modification_date,
                   file_handle* handle
                   )
    {
        // The file_handle should not be open
        assert(handle->self == 0);
        
        handle->pos = static_cast<std::streamsize>(output_file.tellp()) + local_file_header::crc32_offset;
        
        std::vector<char> header(30);
        local_file_header::set_signature(&header[0], local_file_header::signature);
        local_file_header::set_minimum_required_version(&header[0], minimum_required_version);
        local_file_header::set_flags(&header[0], flags);
        local_file_header::set_compression_method(&header[0], compression_method);

        local_file_header::set_filename_size(&header[0], path_size);
        // TODO: handle Zip64
        header.insert(header.end(), path, path + path_size);

        output_file.write(&header[0], header.size());

        std::size_t offset = central_directory.size();
        central_directory.resize(offset + 46);
        central_directory_entry::set_signature(&central_directory[offset], central_directory_entry::signature);
        central_directory_entry::set_creator_version(&central_directory[offset], creator_version);
        central_directory_entry::set_minimum_required_version(&central_directory[offset], minimum_required_version);
        central_directory_entry::set_flags(&central_directory[offset], flags);
        central_directory_entry::set_compression_method(&central_directory[offset], compression_method);
        central_directory_entry::set_modification_time(&central_directory[offset], modification_time);
        central_directory_entry::set_modification_date(&central_directory[offset], modification_date);
        central_directory_entry::set_filename_size(&central_directory[offset], path_size);
        central_directory_entry::set_extra_size(&central_directory[offset], 0);
        central_directory_entry::set_comment_size(&central_directory[offset], 0);
        central_directory_entry::set_file_start_disk(&central_directory[offset], 0);
        central_directory_entry::set_internal_attributes(&central_directory[offset], 0);
        central_directory_entry::set_external_attributes(&central_directory[offset], 0);
        central_directory_entry::set_local_header_offset(&central_directory[offset], current_offset);
        central_directory.insert(central_directory.end(), path, path + path_size);
        
        handle->crc.reset();
        handle->offset = offset;
        handle->compressed_size = 0;
        handle->uncompressed_size = 0;
        handle->self = this;

        current_offset += header.size();
        ++num_files;
    }

    void write_file(const std::string& path, const char* contents, std::size_t size) {
        std::vector<char> header(30);
        local_file_header::set_signature(&header[0], local_file_header::signature);
        local_file_header::set_minimum_required_version(&header[0], 10);
        local_file_header::set_flags(&header[0], 0);
        local_file_header::set_compression_method(&header[0], compression_method::none);
        
        crc_32_type crc;
        crc.process_bytes(contents, size);
        local_file_header::set_crc32(&header[0], crc.checksum());
        local_file_header::set_compressed_size(&header[0], size);
        local_file_header::set_uncompressed_size(&header[0], size);
        local_file_header::set_filename_size(&header[0], path.size());
        // TODO: handle Zip64
        header.insert(header.end(), path.begin(), path.end());

        output_file.write(&header[0], header.size());
        output_file.write(contents, size);

        std::size_t offset = central_directory.size();
        central_directory.resize(offset + 46);
        central_directory_entry::set_signature(&central_directory[offset], central_directory_entry::signature);
        central_directory_entry::set_creator_version(&central_directory[offset], 10);
        central_directory_entry::set_minimum_required_version(&central_directory[offset], 10);
        central_directory_entry::set_flags(&central_directory[offset], 0);
        central_directory_entry::set_compression_method(&central_directory[offset], compression_method::none);
        // FIXME: find correct date and time
        central_directory_entry::set_modification_time(&central_directory[offset], 0);
        central_directory_entry::set_modification_date(&central_directory[offset], 0);
        central_directory_entry::set_crc32(&central_directory[offset], crc.checksum());
        central_directory_entry::set_compressed_size(&central_directory[offset], size);
        central_directory_entry::set_uncompressed_size(&central_directory[offset], size);
        central_directory_entry::set_filename_size(&central_directory[offset], path.size());
        central_directory_entry::set_extra_size(&central_directory[offset], 0);
        central_directory_entry::set_comment_size(&central_directory[offset], 0);
        central_directory_entry::set_file_start_disk(&central_directory[offset], 0);
        central_directory_entry::set_internal_attributes(&central_directory[offset], 0);
        central_directory_entry::set_external_attributes(&central_directory[offset], 0);
        central_directory_entry::set_local_header_offset(&central_directory[offset], current_offset);
        central_directory.insert(central_directory.end(), path.begin(), path.end());
        current_offset = current_offset + header.size() + size;

        ++num_files;
    }
    void close() {
        output_file.write(&central_directory[0], central_directory.size());
        
        if(num_files >= 65536) {
            boost::array<char, zip64_end_of_central_directory::size> data;
            zip64_end_of_central_directory::set_signature(&data[0], zip64_end_of_central_directory::signature);
            zip64_end_of_central_directory::set_size(&data[0], zip64_end_of_central_directory::size - 12);
            zip64_end_of_central_directory::set_creator_version(&data[0], 45);
            zip64_end_of_central_directory::set_minimum_required_version(&data[0], 45);
            zip64_end_of_central_directory::set_disk_number(&data[0], 0);
            zip64_end_of_central_directory::set_directory_start_disk(&data[0], 0);
            zip64_end_of_central_directory::set_entries_on_disk(&data[0], num_files);
            zip64_end_of_central_directory::set_total_entries(&data[0], num_files);
            zip64_end_of_central_directory::set_directory_size(&data[0], central_directory.size());
            zip64_end_of_central_directory::set_directory_offset(&data[0], current_offset);
            output_file.write(&data[0], data.size());

            boost::array<char, zip64_end_of_central_directory_locator::size> locator;
            zip64_end_of_central_directory_locator::set_signature(&locator[0], zip64_end_of_central_directory_locator::signature);
            zip64_end_of_central_directory_locator::set_end_of_directory_disk(&locator[0], 0);
            zip64_end_of_central_directory_locator::set_end_of_directory_offset(&locator[0], current_offset + central_directory.size());
            zip64_end_of_central_directory_locator::set_total_disks(&locator[0], 1);
            output_file.write(&locator[0], locator.size());
            
            std::vector<char> end(22);
            end_of_central_directory::set_signature(&end[0], end_of_central_directory::signature);
            end_of_central_directory::set_disk_number(&end[0], 0);
            end_of_central_directory::set_directory_start_disk(&end[0], 0);
            end_of_central_directory::set_entries_on_disk(&end[0], 0xFFFFu);
            end_of_central_directory::set_total_entries(&end[0], 0xFFFFu);
            end_of_central_directory::set_directory_size(&end[0], central_directory.size());
            end_of_central_directory::set_directory_offset(&end[0], current_offset);
            end_of_central_directory::set_comment_length(&end[0], 0);
            output_file.write(&end[0], end.size());
        } else {
            std::vector<char> end(22);
            end_of_central_directory::set_signature(&end[0], end_of_central_directory::signature);
            end_of_central_directory::set_disk_number(&end[0], 0);
            end_of_central_directory::set_directory_start_disk(&end[0], 0);
            end_of_central_directory::set_entries_on_disk(&end[0], num_files);
            end_of_central_directory::set_total_entries(&end[0], num_files);
            end_of_central_directory::set_directory_size(&end[0], central_directory.size());
            end_of_central_directory::set_directory_offset(&end[0], current_offset);
            end_of_central_directory::set_comment_length(&end[0], 0);
            output_file.write(&end[0], end.size());
        }
    }
private:

    std::ostream& output_file;
    std::vector<char> central_directory;
    std::streamsize current_offset;
    std::size_t num_files;

    // little endian
    struct local_file_header {
        static const boost::uint32_t signature = 0x04034b50u;

        BOOST_ZIP_DEFINE_HEADER(signature,                boost::uint32_t, 0);
        BOOST_ZIP_DEFINE_HEADER(minimum_required_version, boost::uint16_t, 4);
        BOOST_ZIP_DEFINE_HEADER(flags,                    boost::uint16_t, 6);
        BOOST_ZIP_DEFINE_HEADER(compression_method,       boost::uint16_t, 8);
        BOOST_ZIP_DEFINE_HEADER(modification_time,        boost::uint16_t, 10);
        BOOST_ZIP_DEFINE_HEADER(modification_date,        boost::uint16_t, 12);
        BOOST_ZIP_DEFINE_HEADER(crc32,                    boost::uint32_t, 14);
        BOOST_ZIP_DEFINE_HEADER(compressed_size,          boost::uint32_t, 18);
        BOOST_ZIP_DEFINE_HEADER(uncompressed_size,        boost::uint32_t, 22);
        BOOST_ZIP_DEFINE_HEADER(filename_size,            boost::uint16_t, 26);
        BOOST_ZIP_DEFINE_HEADER(extra_size,               boost::uint16_t, 28);

        static char* filename(void* header) {
            return static_cast<char*>(header) + 30;
        }
        const char* filename(const void* header) {
            return static_cast<const char*>(header) + 30;
        }
    };

    struct data_descriptor {
        // The signature may or may not be present
        static const boost::uint32_t signature = 0x08074b50u;
        BOOST_ZIP_DEFINE_HEADER(crc32,                    boost::uint32_t, 0);
        BOOST_ZIP_DEFINE_HEADER(compressed_size,          boost::uint32_t, 4);
        BOOST_ZIP_DEFINE_HEADER(uncompressed_size,        boost::uint32_t, 8);
        // FIXME: handle skipping the signature automatically
    };

    // Not implemented Archive decryption header
    // Not implemented Archive extra data record

    struct central_directory_entry {
        static const boost::uint32_t signature = 0x02014b50u;
        BOOST_ZIP_DEFINE_HEADER(signature,                boost::uint32_t, 0);
        BOOST_ZIP_DEFINE_HEADER(creator_version,          boost::uint16_t, 4);
        BOOST_ZIP_DEFINE_HEADER(minimum_required_version, boost::uint16_t, 6);
        BOOST_ZIP_DEFINE_HEADER(flags,                    boost::uint16_t, 8);
        BOOST_ZIP_DEFINE_HEADER(compression_method,       boost::uint16_t, 10);
        BOOST_ZIP_DEFINE_HEADER(modification_time,        boost::uint16_t, 12);
        BOOST_ZIP_DEFINE_HEADER(modification_date,        boost::uint16_t, 14);
        BOOST_ZIP_DEFINE_HEADER(crc32,                    boost::uint32_t, 16);
        BOOST_ZIP_DEFINE_HEADER(compressed_size,          boost::uint32_t, 20);
        BOOST_ZIP_DEFINE_HEADER(uncompressed_size,        boost::uint32_t, 24);
        BOOST_ZIP_DEFINE_HEADER(filename_size,            boost::uint16_t, 28);
        BOOST_ZIP_DEFINE_HEADER(extra_size,               boost::uint16_t, 30);
        BOOST_ZIP_DEFINE_HEADER(comment_size,             boost::uint16_t, 32);
        BOOST_ZIP_DEFINE_HEADER(file_start_disk,          boost::uint16_t, 34);
        BOOST_ZIP_DEFINE_HEADER(internal_attributes,      boost::uint16_t, 36);
        BOOST_ZIP_DEFINE_HEADER(external_attributes,      boost::uint32_t, 38);
        BOOST_ZIP_DEFINE_HEADER(local_header_offset,      boost::uint32_t, 42);

        // TODO: filename, extra, comment
    };

    struct digital_signature {
        static const boost::uint32_t signature = 0x05054b50;

        BOOST_ZIP_DEFINE_HEADER(data_size, boost::uint16_t, 4);
        // TODO: data
    };

    struct zip64_end_of_central_directory {
        static const boost::uint32_t signature = 0x06064b50u;
        // The value stored into the "size of zip64 end of central
        // directory record" should be the size of the remaining
        // record and should not include the leading 12 bytes.
        BOOST_ZIP_DEFINE_HEADER(signature,                boost::uint32_t, 0);
        BOOST_ZIP_DEFINE_HEADER(size,                     boost::uint64_t, 4);
        BOOST_ZIP_DEFINE_HEADER(creator_version,          boost::uint16_t, 12);
        BOOST_ZIP_DEFINE_HEADER(minimum_required_version, boost::uint16_t, 14);
        BOOST_ZIP_DEFINE_HEADER(disk_number,              boost::uint32_t, 16);
        BOOST_ZIP_DEFINE_HEADER(directory_start_disk,     boost::uint32_t, 20);
        BOOST_ZIP_DEFINE_HEADER(entries_on_disk,          boost::uint64_t, 24);
        BOOST_ZIP_DEFINE_HEADER(total_entries,            boost::uint64_t, 32);
        BOOST_ZIP_DEFINE_HEADER(directory_size,           boost::uint64_t, 40);
        BOOST_ZIP_DEFINE_HEADER(directory_offset,         boost::uint64_t, 48);

        static const size_t size = 56;
        // TODO: data
        //           Header ID  -  2 bytes
        //           Data Size  -  4 bytes
    };

    // H
    struct zip64_end_of_central_directory_locator {
        static const boost::uint32_t signature = 0x07064b50;
        BOOST_ZIP_DEFINE_HEADER(signature,                boost::uint32_t, 0);
        BOOST_ZIP_DEFINE_HEADER(end_of_directory_disk,    boost::uint32_t, 4);
        BOOST_ZIP_DEFINE_HEADER(end_of_directory_offset,  boost::uint64_t, 8);
        BOOST_ZIP_DEFINE_HEADER(total_disks,              boost::uint32_t, 16);

        static const size_t size = 20;
    };

    struct end_of_central_directory {
        static const uint32_t signature = 0x06054b50u;
        BOOST_ZIP_DEFINE_HEADER(signature,                boost::uint32_t, 0);
        BOOST_ZIP_DEFINE_HEADER(disk_number,              boost::uint16_t, 4);
        BOOST_ZIP_DEFINE_HEADER(directory_start_disk,     boost::uint16_t, 6);
        BOOST_ZIP_DEFINE_HEADER(entries_on_disk,          boost::uint16_t, 8);
        BOOST_ZIP_DEFINE_HEADER(total_entries,            boost::uint16_t, 10);
        BOOST_ZIP_DEFINE_HEADER(directory_size,           boost::uint32_t, 12);
        BOOST_ZIP_DEFINE_HEADER(directory_offset,         boost::uint32_t, 16);
        BOOST_ZIP_DEFINE_HEADER(comment_length,           boost::uint16_t, 20);
    };
    
public:

    struct version {
        static const boost::uint16_t system_mask   = 0xFF00u;
        static const boost::uint16_t ms_dos        =  0u << 8;
        static const boost::uint16_t amiga         =  1u << 8;
        static const boost::uint16_t open_vms      =  2u << 8;
        static const boost::uint16_t unix_         =  3u << 8;
        static const boost::uint16_t vm_cms        =  4u << 8;
        static const boost::uint16_t atari_st      =  5u << 8;
        static const boost::uint16_t os_2_hpfs     =  6u << 8;
        static const boost::uint16_t macintosh     =  7u << 8;
        static const boost::uint16_t z_system      =  8u << 8;
        static const boost::uint16_t cp_m          =  9u << 8;
        static const boost::uint16_t windows_ntfs  = 10u << 8;
        static const boost::uint16_t mvs           = 11u << 8;
        static const boost::uint16_t vse           = 12u << 8;
        static const boost::uint16_t acorn_risc    = 13u << 8;
        static const boost::uint16_t vfat          = 14u << 8;
        static const boost::uint16_t alternate_mvs = 15u << 8;
        static const boost::uint16_t beos          = 16u << 8;
        static const boost::uint16_t tandem        = 17u << 8;
        static const boost::uint16_t os_400        = 18u << 8;
        static const boost::uint16_t darwin        = 19u << 8;

        // e.g. 62 = ZIP 6.2
        static const boost::uint16_t zip_version_mask = 0xFFu;
        static const boost::uint16_t default_                        = 10;
        static const boost::uint16_t file_is_volume_label            = 11;
        static const boost::uint16_t file_is_folder                  = 20;
        static const boost::uint16_t file_is_compressed_with_deflate = 20;
        static const boost::uint16_t zip64                           = 45;
        // TODO: ...
    };

    struct flags {
        static const boost::uint16_t encrypted                 = 0x1u;

        static const boost::uint16_t imploding_8k_dictionary   = 0x2u;
        static const boost::uint16_t imploding_3_shannon_faro  = 0x4u;
        static const boost::uint16_t deflating_options_mask    = 0x6u;
        static const boost::uint16_t deflating_normal          = 0x0u;
        static const boost::uint16_t deflating_maximum         = 0x2u;
        static const boost::uint16_t deflating_fast            = 0x4u;
        static const boost::uint16_t deflating_super_fast      = 0x6u;
        static const boost::uint16_t lzma_eos                  = 0x2u;

        static const boost::uint16_t has_data_descriptor       = 0x8u;
        static const boost::uint16_t enhanced_deflating        = 0x10;
        static const boost::uint16_t strong_encryption         = 0x20;
        static const boost::uint16_t utf8                      = 0x800;
        static const boost::uint16_t mask_local_header_data    = 0x2000;
    };

    struct compression_method {
        static const boost::uint16_t none        = 0;
        static const boost::uint16_t shrink      = 1;
        static const boost::uint16_t reduce_1    = 2;
        static const boost::uint16_t reduce_2    = 3;
        static const boost::uint16_t reduce_3    = 4;
        static const boost::uint16_t reduce_4    = 5;
        static const boost::uint16_t implode     = 6;
        static const boost::uint16_t tokenizing  = 7;
        static const boost::uint16_t deflate     = 8;
        static const boost::uint16_t deflate64   = 9;
        static const boost::uint16_t pkware_dcli = 10;
        static const boost::uint16_t bzip2       = 12;
        static const boost::uint16_t lzma        = 14;
        static const boost::uint16_t ibm_terse   = 18;
        static const boost::uint16_t lz77        = 19;
        static const boost::uint16_t wavpack     = 97;
        static const boost::uint16_t ppmd_i_1    = 98;
    };

    struct internal_attributes {
        static const boost::uint16_t ascii = 0x1;
    };

    struct header_id {
        static const boost::uint16_t zip64                         = 0x0001;
        static const boost::uint16_t av_info                       = 0x0007;
        //static const boost::uint16_t extended_language_encoding    = 0x0008;
        static const boost::uint16_t os_2                          = 0x0009;
        static const boost::uint16_t ntfs                          = 0x000a;
        static const boost::uint16_t open_vms                      = 0x000c;
        static const boost::uint16_t unix_                         = 0x000d;
        //static const boost::uint16_t file_stream                   = 0x000e;
        static const boost::uint16_t patch_descriptor              = 0x000f;
        static const boost::uint16_t x509_certificate              = 0x0014;
        static const boost::uint16_t x509_certificate_id_file      = 0x0015;
        static const boost::uint16_t x509_certificate_id_directory = 0x0016;
        static const boost::uint16_t strong_encryption_header      = 0x0017;
        static const boost::uint16_t record_management_controls    = 0x0018;
        static const boost::uint16_t encyption_recipients          = 0x0019;
        static const boost::uint16_t ibm_uncompressed              = 0x0065;
        static const boost::uint16_t ibm_compressed                = 0x0066;
        static const boost::uint16_t poszip4690                    = 0x4690;

        // TODO: Third party mappings
    };

private:
    struct zip64_extended_information {
        BOOST_ZIP_DEFINE_HEADER(tag,                      boost::uint16_t, 0);
        BOOST_ZIP_DEFINE_HEADER(size,                     boost::uint16_t, 2);
        BOOST_ZIP_DEFINE_HEADER(uncompressed_size,        boost::uint64_t, 4);
        BOOST_ZIP_DEFINE_HEADER(compressed_size,          boost::uint64_t, 12);
        BOOST_ZIP_DEFINE_HEADER(local_header_offset,      boost::uint64_t, 20);
        BOOST_ZIP_DEFINE_HEADER(disk_start_number,        boost::uint32_t, 28);
    };
};

class shrink_filter : ::boost::noncopyable {
public:
    typedef char char_type;

    struct category :
        ::boost::iostreams::output_filter_tag,
        ::boost::iostreams::closable_tag
    {};

    shrink_filter()
    {
        memory = new lzw_node[1 << 13];

        // no-throw from here on
        code_size = 9;
        for(int i = 0; i < (1 << code_size); ++i) {
            initialize_node(i);
        }
        used_codes.set(256);
        current_node = &root;
        buf = 0;
        pos = 0;
        for(int i = 0; i < 256; ++i) {
            root.children[i] = make_node(i);
        }
        next_code = 257;
    }
    ~shrink_filter() {
        delete[] memory;
    }
    template<class Sink>
    bool put(Sink& sink, char ch) {
        write_char(static_cast<unsigned char>(ch));
        return do_write(sink);
    }
    template<class Sink>
    void close(Sink& sink) {
        if(current_node != &root) {
            write_code(get_encoding(current_node));
            current_node = &root;
        }
        do_write(sink);
        if(pos != 0) {
            ::boost::iostreams::put(sink, buf & 0xFF);
            pos = 0;
        }
    }
private:
    template<class Sink>
    bool do_write(Sink& sink) {
        while(pos >= 8) {
            if(!::boost::iostreams::put(sink, static_cast<char>(buf & 0xFF))) {
                return false;
            }
            buf >>= 8;
            pos -= 8;
        }
        return true;
    }
    void write_char(unsigned char ch) {
        if(current_node->children[ch] != 0) {
            current_node = current_node->children[ch];
        } else {
            int encoding = get_encoding(current_node);
            write_code(encoding);
            for(;; ++next_code) {
                if(next_code == (1 << code_size)) {
                    if(code_size == 13) {
                        write_code(256);
                        write_code(2);
                        free_leaves();
                        next_code = 257;
                    } else {
                        write_code(256);
                        write_code(1);
                        increment_code_size();
                    }
                }
                if(!used_codes.test(next_code)) {
                    current_node->children[ch] = make_node(next_code);
                    ++next_code;
                    break;
                }
            }
            current_node = root.children[ch];
        }
    }
    void write_code(int code) {
        buf |= static_cast<boost::uint64_t>(code) << pos;
        pos += code_size;
    }

    struct lzw_node {
        lzw_node* children[256];
    };
    int get_encoding(lzw_node* node) const {
        return node - memory;
    }
    bool free_leaves(lzw_node* node) {
        bool result = true;
        for(int i = 0; i < 256; ++i) {
            if(node->children[i] != 0) {
                result = false;
                if(free_leaves(node->children[i])) {
                    destroy_node(node->children[i]);
                    node->children[i] = 0;
                }
            }
        }
        return result;
    }
    void increment_code_size() {
        for(int i = (1 << code_size); i < (1 << (code_size + 1)); ++i) {
            initialize_node(i);
        }
        ++code_size;
    }
    void free_leaves() {
        for(int i = 0; i < 256; ++i) {
            free_leaves(root.children[i]);
        }
    }
    void initialize_node(int encoding) {
        lzw_node* result = memory + encoding;
        for(int i = 0; i < 256; ++i) {
            result->children[i] = 0;
        }
    }
    lzw_node* make_node(int encoding = 0) {
        assert(!used_codes.test(encoding));
        lzw_node* result = memory + encoding;
        assert(result >= memory);
        assert(result < memory + (1 << code_size));
        used_codes.set(encoding);
        return result;
    }
    void destroy_node(lzw_node* node) {
        used_codes.reset(get_encoding(node));
    }
    lzw_node* memory;
    lzw_node root;
    lzw_node* current_node;
    int code_size;
    int next_code;
    ::std::bitset<(1 << 13)> used_codes;
    ::boost::uint64_t buf;
    int pos;
};

class deflate_filter : public ::boost::iostreams::zlib_compressor {
public:
    deflate_filter() :
        boost::iostreams::zlib_compressor(boost::iostreams::zlib_params(
            boost::iostreams::zlib::default_compression,
            boost::iostreams::zlib::deflated,
            boost::iostreams::zlib::default_window_bits,
            boost::iostreams::zlib::default_mem_level,
            boost::iostreams::zlib::default_strategy,
            true /* noheader */,
            false /* crc */))
    {}
};

class noop_filter
{
public:
    typedef char char_type;

    struct category :
        ::boost::iostreams::output_filter_tag,
        ::boost::iostreams::multichar_tag
    {};

    template<class Device>
    std::streamsize write(Device& dev, const char * data, std::streamsize size) {
        return boost::iostreams::write(dev, data, size);
    }
};

template<class Filter>
struct compression_method;

template<>
struct compression_method< ::boost::zip::noop_filter> :
    ::boost::mpl::integral_c<
        ::boost::uint16_t,
        ::boost::zip::zip_archive::compression_method::none
    >
{};

template<>
struct compression_method< ::boost::zip::shrink_filter> :
    ::boost::mpl::integral_c<
        ::boost::uint16_t,
        ::boost::zip::zip_archive::compression_method::shrink
    >
{};

template<>
struct compression_method< ::boost::zip::deflate_filter> :
    ::boost::mpl::integral_c<
        ::boost::uint16_t,
        ::boost::zip::zip_archive::compression_method::deflate
    >
{};

template<class Filter>
class zip_member_sink {
public:
    typedef char char_type;

    struct category :
        ::boost::iostreams::sink_tag,
        ::boost::iostreams::closable_tag
    {};

    zip_member_sink(zip_archive& archive, const std::string& path)
      : file(archive, path, 10, 10, 0,
             compression_method<Filter>::value) {}
    ~zip_member_sink() {
        close();
    }
    ::std::streamsize write(const char* data, ::std::streamsize size) {
        file.write_uncompressed(data, size);
        ::boost::iostreams::write(filter, file, data, size);
        return size;
    }
    void close() {
        ::boost::iostreams::close(filter, file, ::std::ios_base::out);
        ::boost::iostreams::close(file);
    }
private:
    zip_archive::file_handle file;
    Filter filter;
};

typedef zip_member_sink<shrink_filter> shrink_sink;
typedef zip_member_sink<deflate_filter> deflate_sink;
typedef zip_member_sink<noop_filter> nocompression_sink;

}
}

#endif
