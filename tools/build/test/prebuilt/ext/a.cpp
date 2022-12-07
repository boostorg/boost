//  Copyright (c) 2003 Vladimir Prus
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE.txt or copy at
//  https://www.bfgroup.xyz/b2/LICENSE.txt)

#ifdef _WIN32
__declspec(dllexport)
#endif
#ifdef RELEASE
void release() {}
#else
void debug() {}
#endif
