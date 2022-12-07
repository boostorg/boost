//  Copyright (c) 2003 Vladimir Prus
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE.txt or copy at
//  https://www.bfgroup.xyz/b2/LICENSE.txt)

#include <a.h>

int main()
{
    #ifdef RELEASE
    release();
    #else
    debug();
    #endif
    return 0;
}
