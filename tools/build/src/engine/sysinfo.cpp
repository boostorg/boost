/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "sysinfo.h"
#include "jam.h"
#include "output.h"

#include <thread>

#if defined(OS_MACOSX)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if !defined(OS_NT)
#include <unistd.h>
#else
#include <windows.h>
#endif

#if defined(OS_LINUX) || defined(__GLIBC__)
// Need to define this in case it's not as that's the only way to get the
// sched_* APIs.
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sched.h>
#endif


b2::system_info::system_info()
{
}

namespace
{
    unsigned int macosx_physicalcpu()
    {
        #if defined(OS_MACOSX)
        int out_hw_ncpu = 0;
        size_t len_hw_ncpu = sizeof(out_hw_ncpu);
        int result = ::sysctlbyname(
            "hw.physicalcpu", &out_hw_ncpu, &len_hw_ncpu, nullptr, 0);
        if (result == 0) return out_hw_ncpu;
        #endif
        return 0;
    }

    unsigned int macosx_logicalcpu()
    {
        #if defined(OS_MACOSX)
        int out_hw_ncpu = 0;
        size_t len_hw_ncpu = sizeof(out_hw_ncpu);
        int result = ::sysctlbyname(
            "hw.logicalcpu", &out_hw_ncpu, &len_hw_ncpu, nullptr, 0);
        if (result == 0) return out_hw_ncpu;
        #endif
        return 0;
    }

    unsigned int sched_affinity_cpu_count()
    {
        #if defined(CPU_COUNT_S)
        ::cpu_set_t cpu_set;
        if (::sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set) == 0)
        {
            return CPU_COUNT_S(sizeof(cpu_set_t), &cpu_set);
        }
        #endif
        return 0;
    }

    unsigned int sysconf_nprocs_configured()
    {
        #if defined(_SC_NPROCESSORS_ONLN)
        return ::sysconf(_SC_NPROCESSORS_CONF);
        #else
        return 0;
        #endif
    }

    unsigned int sysconf_nprocs_online()
    {
        #if defined(_SC_NPROCESSORS_ONLN)
        return ::sysconf(_SC_NPROCESSORS_ONLN);
        #else
        return 0;
        #endif
    }

    unsigned int std_thread_hardware_concurrency()
    {
        // [2020-08-19] Mingw-w64 (e.g. i686-w64-mingw-32-g++ from Cygwin,
        // g++-mingw-w64-i686-win32) does not have std::thread etc. But we
        // should still allow building the engine with this (important) toolset:
        // - It is free, lightweight, standards-conforming.
        // - It might be the only C++11 toolset for Windows XP.
        //   (Please see http://www.crouchingtigerhiddenfruitbat.org/Cygwin/timemachine.html !)
        // - It is powerful enough even without std::thread etc. For example, it
        //   can build-and-use Boost.Thread.
        // - The only thing currently used from std::thread is this call to
        //   hardware_concurrency !
        #if ! defined (_WIN32)
        return std::thread::hardware_concurrency();
        #else
        return 0;
        #endif
    }

    unsigned int win32_logicalcpu()
    {
        #if defined (_WIN32)
        SYSTEM_INFO si;
        GetSystemInfo (&si);
        return si.dwNumberOfProcessors;
        #else
        return 0;
        #endif
    }
}

unsigned int b2::system_info::cpu_core_count()
{
    if (cpu_core_count_ == 0)
    {
        cpu_thread_count_ = macosx_physicalcpu();
    }
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = sysconf_nprocs_configured();
    }
    if (cpu_core_count_ <= 0)
    {
        cpu_core_count_ = 1;
    }
    return cpu_core_count_;
}

unsigned int b2::system_info::cpu_thread_count()
{
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = macosx_logicalcpu();
    }
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = sched_affinity_cpu_count();
    }
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = sysconf_nprocs_online();
    }
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = std_thread_hardware_concurrency();
    }
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = win32_logicalcpu();
    }
    if (cpu_thread_count_ == 0)
    {
        cpu_thread_count_ = cpu_core_count();
    }
    return cpu_thread_count_;
}
