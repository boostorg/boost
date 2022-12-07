/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/* This file is ALSO:
 * Copyright 2001-2004 David Abrahams.
 * Copyright 2005 Rene Rivera.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

/*
 * pathsys.c - platform independent path manipulation support
 *
 * External routines:
 *  path_build()   - build a filename given dir/base/suffix/member
 *  path_parent()  - make a PATHNAME point to its parent dir
 *  path_parse()   - split a file name into dir/base/suffix/member
 *  path_tmpdir()  - returns the system dependent temporary folder path
 *  path_tmpfile() - returns a new temporary path
 *  path_tmpnam()  - returns a new temporary name
 *
 * File_parse() and path_build() just manipulate a string and a structure;
 * they do not make system calls.
 */

#include "jam.h"

#include "cwd.h"
#include "filesys.h"
#include "pathsys.h"

#include <stdlib.h>
#include <time.h>

#include <algorithm>


/* Internal OS specific implementation details - have names ending with an
 * underscore and are expected to be implemented in an OS specific pathXXX.c
 * module.
 */
unsigned long path_get_process_id_( void );
void path_get_temp_path_( string * buffer );
int path_translate_to_os_( char const * f, string * file );


/*
 * path_parse() - split a file name into dir/base/suffix/member
 */

void path_parse( char const * file, PATHNAME * f )
{
    char const * p;
    char const * q;
    char const * end;

    memset( (char *)f, 0, sizeof( *f ) );

    /* Look for '<grist>'. */

    if ( ( file[ 0 ] == '<' ) && ( p = strchr( file, '>' ) ) )
    {
        f->f_grist.ptr = file;
        f->f_grist.len = int32_t(p - file);
        file = p + 1;
    }

    /* Look for 'dir/'. */

    p = strrchr( file, '/' );

#if PATH_DELIM == '\\'
    /* On NT, look for dir\ as well */
    {
        char const * p1 = strrchr( p ? p + 1 : file, '\\' );
        if ( p1 ) p = p1;
    }
#endif

    if ( p )
    {
        f->f_dir.ptr = file;
        f->f_dir.len = int32_t(p - file);

        /* Special case for / - dirname is /, not "" */
        if ( !f->f_dir.len )
            ++f->f_dir.len;

#if PATH_DELIM == '\\'
        /* Special case for D:/ - dirname is D:/, not "D:" */
        if ( f->f_dir.len == 2 && file[ 1 ] == ':' )
            ++f->f_dir.len;
#endif

        file = p + 1;
    }

    end = file + strlen( file );

    /* Look for '(member)'. */
    if ( ( p = strchr( file, '(' ) ) && ( end[ -1 ] == ')' ) )
    {
        f->f_member.ptr = p + 1;
        f->f_member.len = int32_t(end - p - 2);
        end = p;
    }

    /* Look for '.suffix'. This would be memrchr(). */
    p = 0;
    for ( q = file; ( q = (char *)memchr( q, '.', end - q ) ); ++q )
        p = q;
    if ( p )
    {
        f->f_suffix.ptr = p;
        f->f_suffix.len = int32_t(end - p);
        end = p;
    }

    /* Leaves base. */
    f->f_base.ptr = file;
    f->f_base.len = int32_t(end - file);
}


/*
 * is_path_delim() - true iff c is a path delimiter
 */

static int is_path_delim( char const c )
{
    return c == PATH_DELIM
#if PATH_DELIM == '\\'
        || c == '/'
#endif
        ;
}


/*
 * as_path_delim() - convert c to a path delimiter if it is not one already
 */

static char as_path_delim( char const c )
{
    return is_path_delim( c ) ? c : PATH_DELIM;
}


/*
 * path_build() - build a filename given dir/base/suffix/member
 *
 * To avoid changing slash direction on NT when reconstituting paths, instead of
 * unconditionally appending PATH_DELIM we check the past-the-end character of
 * the previous path element. If it is a path delimiter, we append that, and
 * only append PATH_DELIM as a last resort. This heuristic is based on the fact
 * that PATHNAME objects are usually the result of calling path_parse, which
 * leaves the original slashes in the past-the-end position. Correctness depends
 * on the assumption that all strings are zero terminated, so a past-the-end
 * character will always be available.
 *
 * As an attendant patch, we had to ensure that backslashes are used explicitly
 * in 'timestamp.c'.
 */

void path_build( PATHNAME * f, string * file )
{
    int check_f;
    int check_f_pos;

    file_build1( f, file );

    /* Do not prepend root if it is '.' or the directory is rooted. */
    check_f = (f->f_root.len
               && !( f->f_root.len == 1 && f->f_root.ptr[ 0 ] == '.')
               && !( f->f_dir.len && f->f_dir.ptr[ 0 ] == '/' ));
#if PATH_DELIM == '\\'
    check_f = (check_f
               && !( f->f_dir.len && f->f_dir.ptr[ 0 ] == '\\' )
               && !( f->f_dir.len && f->f_dir.ptr[ 1 ] == ':' ));
#endif
    if (check_f)
    {
        string_append_range( file, f->f_root.ptr, f->f_root.ptr + f->f_root.len
            );
        /* If 'root' already ends with a path delimiter, do not add another one.
         */
        if ( !is_path_delim( f->f_root.ptr[ f->f_root.len - 1 ] ) )
            string_push_back( file, as_path_delim( f->f_root.ptr[ f->f_root.len
                ] ) );
    }

    if ( f->f_dir.len )
        string_append_range( file, f->f_dir.ptr, f->f_dir.ptr + f->f_dir.len );

    /* Put path separator between dir and file. */
    /* Special case for root dir: do not add another path separator. */
    check_f_pos = (f->f_dir.len && ( f->f_base.len || f->f_suffix.len ));
#if PATH_DELIM == '\\'
    check_f_pos = (check_f_pos && !( f->f_dir.len == 3 && f->f_dir.ptr[ 1 ] == ':' ));
#endif
    check_f_pos = (check_f_pos && !( f->f_dir.len == 1 && is_path_delim( f->f_dir.ptr[ 0 ])));
    if (check_f_pos)
        string_push_back( file, as_path_delim( f->f_dir.ptr[ f->f_dir.len ] ) );

    if ( f->f_base.len )
        string_append_range( file, f->f_base.ptr, f->f_base.ptr + f->f_base.len
            );

    if ( f->f_suffix.len )
        string_append_range( file, f->f_suffix.ptr, f->f_suffix.ptr +
            f->f_suffix.len );

    if ( f->f_member.len )
    {
        string_push_back( file, '(' );
        string_append_range( file, f->f_member.ptr, f->f_member.ptr +
            f->f_member.len );
        string_push_back( file, ')' );
    }
}


/*
 * path_parent() - make a PATHNAME point to its parent dir
 */

void path_parent( PATHNAME * f )
{
    f->f_base.ptr = f->f_suffix.ptr = f->f_member.ptr = "";
    f->f_base.len = f->f_suffix.len = f->f_member.len = 0;
}


/*
 * path_tmpdir() - returns the system dependent temporary folder path
 *
 * Returned value is stored inside a static buffer and should not be modified.
 * Returned value does *not* include a trailing path separator.
 */

string const * path_tmpdir()
{
    static string buffer[ 1 ];
    static int have_result;
    if ( !have_result )
    {
        string_new( buffer );
        path_get_temp_path_( buffer );
        have_result = 1;
    }
    return buffer;
}


/*
 * path_tmpnam() - returns a new temporary name
 */

OBJECT * path_tmpnam( void )
{
    char name_buffer[ 64 ];
    unsigned long const pid = path_get_process_id_();
    static unsigned long t;
    if ( !t ) t = time( 0 ) & 0xffff;
    t += 1;
    sprintf( name_buffer, "jam%lx%lx.000", pid, t );
    return object_new( name_buffer );
}


/*
 * path_tmpfile() - returns a new temporary path
 */

OBJECT * path_tmpfile( void )
{
    OBJECT * result;
    OBJECT * tmpnam;

    string file_path[ 1 ];
    string_copy( file_path, path_tmpdir()->value );
    string_push_back( file_path, PATH_DELIM );
    tmpnam = path_tmpnam();
    string_append( file_path, object_str( tmpnam ) );
    object_free( tmpnam );
    result = object_new( file_path->value );
    string_free( file_path );

    return result;
}


/*
 * path_translate_to_os() - translate filename to OS-native path
 *
 */

int path_translate_to_os( char const * f, string * file )
{
  return path_translate_to_os_( f, file );
}


std::string b2::paths::normalize(const std::string &p)
{
    // We root the path as a sentinel. But we need to remember that we did so
    // to un-root afterwards.
    std::string result{"/"};
    bool is_rooted = p[0] == '/' || p[0] == '\\';
    result += p;

    // Convert \ into /. On Windows, paths using / and \ are equivalent, and we
    // want this function to obtain a canonic representation.
    std::replace(result.begin(), result.end(), '\\', '/');

    int32_t ellipsis = 0;
    for (auto end_pos = result.length(); end_pos > 0; )
    {
        auto path_pos = result.rfind('/', end_pos-1);
        if (path_pos == std::string::npos) break;
        if (path_pos == end_pos-1)
        {
            /* Found a trailing or duplicate '/'. Remove it. */
            result.erase(path_pos, 1);
        }
        else if ((end_pos-path_pos == 2) && result[path_pos+1] == '.')
        {
            /* Found '/.'. Remove them all. */
            result.erase(path_pos, 2);
        }
        else if ((end_pos-path_pos == 3) && result[path_pos+1] == '.' && result[path_pos+2] == '.')
        {
            /* Found '/..'. Remove them all. */
            result.erase(path_pos, 3);
            ellipsis += 1;
        }
        else if (ellipsis > 0)
        {
            /* An elided parent path. Remove it. */
            result.erase(path_pos, end_pos-path_pos);
            ellipsis -= 1;
        }
        end_pos = path_pos;
    }

    // Now we know that we need to add exactly ellipsis '..' path elements to the
    // front and that our string is either empty or has a '/' as its first
    // significant character. If we have any ellipsis remaining then the passed
    // path must not have been rooted or else it is invalid we return empty.
    if (ellipsis > 0)
    {
        if (is_rooted) return "";
        do result.insert(0, "/.."); while (--ellipsis > 0);
    }

    // If we reduced to nothing we return a valid path depending on wether
    // the input was rooted or not.
    if (result.empty()) return is_rooted ? "/" : ".";
    // Return the result without the sentinel if it's not rooted.
    if (!is_rooted) return result.substr(1);

    return result;
}


/*
 * executable_path()
 */

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
char * executable_path( char const * argv0 )
{
    char buf[ 1024 ];
    DWORD const ret = GetModuleFileNameA( NULL, buf, sizeof( buf ) );
    return ( !ret || ret == sizeof( buf ) ) ? NULL : strdup( buf );
}
#elif defined(__APPLE__)  /* Not tested */
# include <mach-o/dyld.h>
char *executable_path( char const * argv0 )
{
    char buf[ 1024 ];
    uint32_t size = sizeof( buf );
    return _NSGetExecutablePath( buf, &size ) ? NULL : strdup( buf );
}
#elif defined(sun) || defined(__sun)  /* Not tested */
# include <stdlib.h>
char * executable_path( char const * argv0 )
{
    const char * execname = getexecname();
    return execname ? strdup( execname ) : NULL;
}
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
# include <sys/sysctl.h>
char * executable_path( char const * argv0 )
{
    int mib[ 4 ] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    char buf[ 1024 ];
    size_t size = sizeof( buf );
    sysctl( mib, 4, buf, &size, NULL, 0 );
    return ( !size || size == sizeof( buf ) ) ? NULL : strndup( buf, size );
}
#elif defined(__linux__) || defined(__CYGWIN__) || defined(__GNU__)
# include <unistd.h>
char * executable_path( char const * argv0 )
{
    char buf[ 1024 ];
    ssize_t const ret = readlink( "/proc/self/exe", buf, sizeof( buf ) );
    return ( !ret || ret == sizeof( buf ) ) ? NULL : strndup( buf, ret );
}
#elif defined(OS_VMS)
# include <unixlib.h>
char * executable_path( char const * argv0 )
{
    char * vms_path = NULL;
    char * posix_path = NULL;
    char * p;

    /* On VMS argv[0] shows absolute path to the image file.
     * So, just remove VMS file version and translate path to POSIX-style.
     */
    vms_path = strdup( argv0 );
    if ( vms_path && ( p = strchr( vms_path, ';') ) ) *p = '\0';
    posix_path = decc$translate_vms( vms_path );
    if ( vms_path ) free( vms_path );

    return posix_path > 0 ? strdup( posix_path ) : NULL;
}
#else
char * executable_path( char const * argv0 )
{
    char * result = nullptr;
    /* If argv0 is an absolute path, assume it is the right absolute path. */
    if (!result && b2::paths::is_rooted(argv0))
        result = strdup( argv0 );
    // If argv0 is a relative path, we can compute the absolute one from the
    // current working dir.
    if (!result && b2::paths::is_relative(argv0))
    {
        auto p = b2::paths::normalize(b2::cwd_str()+"/"+argv0);
        result = strdup( p.c_str() );
    }
    // If it's a bare basename, search the PATH for a match.
    if (!result)
    {
        std::string path_env = getenv( "PATH" );
        std::string::size_type i = 0;
        while (i != std::string::npos)
        {
            std::string::size_type e = path_env.find_first_of(':', i);
            std::string p = e == std::string::npos
                ? path_env.substr(i)
                : path_env.substr(i, e-i);
            if (b2::filesys::is_file(p+"/"+argv0))
            {
                result = strdup( (p+"/"+argv0).c_str() );
                break;
            }
            i = e == std::string::npos ? e : e+1;
        }
    }
    return result;
}
#endif
