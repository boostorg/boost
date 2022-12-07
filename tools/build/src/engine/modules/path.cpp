/* Copyright Vladimir Prus 2003.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "../constants.h"
#include "../frames.h"
#include "../lists.h"
#include "../native.h"
#include "../filesys.h"


LIST * path_exists( FRAME * frame, int flags )
{
    return file_query( list_front( lol_get( frame->args, 0 ) ) ) ?
        list_new( object_copy( constant_true ) ) : L0;
}


void init_path()
{
    char const * args[] = { "location", 0 };
    declare_native_rule( "path", "exists", args, path_exists, 1 );
}
