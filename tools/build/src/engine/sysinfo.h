/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#ifndef B2_SYSINFO_H
#define B2_SYSINFO_H

# include "config.h"

namespace b2
{
    /*
    Provides information about the system, hardware and software, we are
    running in.
    */
    class system_info
    {
        public:

        system_info();

        /*
        Returns the number of physical CPU cores if available. Otherwise
        returns 1.

        Currently implemented for: OS_MACOSX.
        */
        unsigned int cpu_core_count();

        /*
        Returns the number of logical CPU threads is available. Otherwise
        returns `spu_core_count()`.

        Currently implemented for: OS_MACOSX.
        */
        unsigned int cpu_thread_count();

        private:

        unsigned int cpu_core_count_ = 0;
        unsigned int cpu_thread_count_ = 0;
    };
}

#endif
