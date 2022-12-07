/*
 * Copyright 2006-2022 René Ferdinand Rivera Morell
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#ifndef BJAM_MEM_H
#define BJAM_MEM_H

#include "config.h"

#include <cstdlib>
#include <memory>

#define bjam_malloc_x(s) std::malloc(s)
#define bjam_calloc_x(n, s) std::calloc(n, s)
#define bjam_realloc_x(p, s) std::realloc(p, s)
#define bjam_free_x(p) std::free(p)

#ifndef bjam_malloc_atomic_x
#define bjam_malloc_atomic_x(s) bjam_malloc_x(s)
#endif
#ifndef bjam_calloc_atomic_x
#define bjam_calloc_atomic_x(n, s) bjam_calloc_x(n, s)
#endif
#ifndef bjam_mem_init_x
#define bjam_mem_init_x()
#endif
#ifndef bjam_mem_close_x
#define bjam_mem_close_x()
#endif
#ifndef bjam_malloc_raw_x
#define bjam_malloc_raw_x(s) bjam_malloc_x(s)
#endif
#ifndef bjam_calloc_raw_x
#define bjam_calloc_raw_x(n, s) bjam_calloc_x(n, s)
#endif
#ifndef bjam_realloc_raw_x
#define bjam_realloc_raw_x(p, s) bjam_realloc_x(p, s)
#endif
#ifndef bjam_free_raw_x
#define bjam_free_raw_x(p) bjam_free_x(p)
#endif

#ifdef OPT_DEBUG_PROFILE
/* Profile tracing of memory allocations. */
#include "debug.h"

#define BJAM_MALLOC(s) (profile_memory(s), bjam_malloc_x(s))
#define BJAM_MALLOC_ATOMIC(s) (profile_memory(s), bjam_malloc_atomic_x(s))
#define BJAM_CALLOC(n, s) (profile_memory(n * s), bjam_calloc_x(n, s))
#define BJAM_CALLOC_ATOMIC(n, s) (profile_memory(n * s), bjam_calloc_atomic_x(n, s))
#define BJAM_REALLOC(p, s) (profile_memory(s), bjam_realloc_x(p, s))

#define BJAM_MALLOC_RAW(s) (profile_memory(s), bjam_malloc_raw_x(s))
#define BJAM_CALLOC_RAW(n, s) (profile_memory(n * s), bjam_calloc_raw_x(n, s))
#define BJAM_REALLOC_RAW(p, s) (profile_memory(s), bjam_realloc_raw_x(p, s))
#else
/* No mem tracing. */
#define BJAM_MALLOC(s) bjam_malloc_x(s)
#define BJAM_MALLOC_ATOMIC(s) bjam_malloc_atomic_x(s)
#define BJAM_CALLOC(n, s) bjam_calloc_x(n, s)
#define BJAM_CALLOC_ATOMIC(n, s) bjam_calloc_atomic_x(n, s)
#define BJAM_REALLOC(p, s) bjam_realloc_x(p, s)

#define BJAM_MALLOC_RAW(s) bjam_malloc_raw_x(s)
#define BJAM_CALLOC_RAW(n, s) bjam_calloc_raw_x(n, s)
#define BJAM_REALLOC_RAW(p, s) bjam_realloc_raw_x(p, s)
#endif

#define BJAM_MEM_INIT() bjam_mem_init_x()
#define BJAM_MEM_CLOSE() bjam_mem_close_x()

#define BJAM_FREE(p) bjam_free_x(p)
#define BJAM_FREE_RAW(p) bjam_free_raw_x(p)

namespace b2 {
namespace jam {

    template <typename T, typename... Args>
    T* ctor_ptr(void* p, Args&&... args)
    {
        std::memset(p, 0, sizeof(T));
        return new (p) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void dtor_ptr(T* p)
    {
        p->~T();
    }

    template <typename T, typename... Args>
    T* make_ptr(Args&&... args)
    {
        return ctor_ptr<T>(BJAM_MALLOC(sizeof(T)));
    }

    template <typename T>
    void free_ptr(T* p)
    {
        dtor_ptr(p);
        BJAM_FREE(p);
    }

    template <typename T>
    struct unique_jptr_deleter {
        void operator()(T* p) const
        {
            p->~T();
            BJAM_FREE(p);
        }
    };

    template <typename T>
    using unique_jptr = std::unique_ptr<T, unique_jptr_deleter<T>>;

    template <typename T, typename... Args>
    unique_jptr<T> make_unique_jptr(Args&&... args)
    {
        return unique_jptr<T>(make_ptr<T>(std::forward<Args>(args)...));
    }

    template <typename T>
    struct unique_jptr_free {
        using free_f = void (*)(T*);

        explicit unique_jptr_free(free_f f)
            : free_function(f)
        {
        }

        unique_jptr_free(unique_jptr_free&& p)
            : free_function(std::move(p.free_function))
        {
            p.free_function = nullptr;
        }

        void operator()(T* p) const
        {
            // if (free_function)
            if (p)
                (*free_function)(p);
        }

    private:
        free_f free_function = nullptr;
    };

    template <typename T>
    using unique_bare_jptr = std::unique_ptr<T, unique_jptr_free<T>>;

    template <typename T, typename F>
    unique_bare_jptr<T> make_unique_bare_jptr(T* p, F exit_f)
    {
        return unique_bare_jptr<T>(p, unique_jptr_free<T>(exit_f));
    }

    template <typename T, typename F, typename G>
    unique_bare_jptr<T> make_unique_bare_jptr(T* p, F enter_f, G exit_f)
    {
        enter_f(p);
        return unique_bare_jptr<T>(p, unique_jptr_free<T>(exit_f));
    }

}
}

#endif
