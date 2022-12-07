
//          Copyright Oliver Kowalke 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/numa/pin_thread.hpp"

extern "C" {
#include <windows.h>
}

#include <cstring>
#include <system_error>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace numa {

BOOST_FIBERS_DECL
void pin_thread( std::uint32_t cpuid) {
#ifdef __MINGW32__
    // Workaround for bug in mingw-w64: https://sourceforge.net/p/mingw-w64/bugs/908
    pin_thread( cpuid, reinterpret_cast<std::thread::native_handle_type>(::GetCurrentThread()) );
#else
    pin_thread( cpuid, ::GetCurrentThread() );
#endif
}

BOOST_FIBERS_DECL
void pin_thread( std::uint32_t cpuid, std::thread::native_handle_type h) {
    GROUP_AFFINITY affinity;
    std::memset( & affinity, 0, sizeof( affinity) );
    // compute processor group
    // a group contains max 64 logical CPUs
    affinity.Group = static_cast< WORD >(cpuid / 64);
    // compute the ID of the logical CPU in the group
    uint32_t id = cpuid % 64; 
    // set the bit mask of the logical CPU
    affinity.Mask = static_cast< KAFFINITY >( 1) << id;
#ifdef __MINGW32__
    // Workaround for bug in mingw-w64: https://sourceforge.net/p/mingw-w64/bugs/908
    if ( BOOST_UNLIKELY( 0 == ::SetThreadGroupAffinity( reinterpret_cast<HANDLE>(h), & affinity, nullptr) ) ) {
#else
    if ( BOOST_UNLIKELY( 0 == ::SetThreadGroupAffinity( h, & affinity, nullptr) ) ) {
#endif
        throw std::system_error(
                std::error_code( ::GetLastError(), std::system_category() ),
                "::SetThreadiGroupAffinity() failed");
    }
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_SUFFIX
#endif
