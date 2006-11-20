========================================
 Getting Started With Boost  |(logo)|__
========================================

.. |(logo)| image:: ../boost.png
   :alt: Boost

__ ../index.htm

This guide will help you get started using the Boost libraries.
Have fun!

.. section-numbering::

.. contents:: Index


.. ## Update this substitution for each release

.. |boost_ver| replace:: ``boost_1_34_0``
.. |boost_ver-bold| replace:: **boost_1_34_0**

.. |root| replace:: ``/``\ *path*\ ``/``\ *to*\ ``/``\ |boost_ver|
.. |winroot| replace:: *C:*\ ``\``\ *path*\ ``\``\ *to*\ ``\``\ |boost_ver|
.. |winroot-default| replace:: ``C:\Program Files\boost\``\ |boost_ver|
.. |bold-winroot-default| replace:: **C:\\Program Files\\boost\\**\ |boost_ver|

Getting Boost
=============

There are basically three ways to get Boost on your system:

1. Download and run the `Windows installer`_ supplied by Boost
   Consulting (not available for Boost alpha/beta releases).

.. ## remove the parenthesized note for full releases
.. _Windows installer: http://www.boost-consulting.com/download.html

2. or, `download a complete Boost distribution`__ from SourceForge.

.. ## Update this link for each release
__ http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=8041&release_id=376197

   :Windows users: |boost_ver|\ ``.exe`` is a program you can
     run to unpack the distribution; if you prefer not to download
     executable programs, get |boost_ver|\ ``.zip`` and use an
     external tool to decompress it.  We don't recommend using
     Windows' built-in decompression as it can be painfully slow
     for large archives.

   :\*nix users: Download |boost_ver|\ ``.tar.bz2``, then, in the
     directory where you want to put the Boost installation,
     execute

     .. parsed-literal::
   
        tar --bzip2 -xf */path/to/*\ |boost_ver|\ .tar.bz2

3. or use a Boost package from RedHat, Debian, or some other
   distribution packager.  These instructions may not work for you
   if you use this method, because other packagers sometimes choose
   to break Boost up into several packages or to reorganize the
   directory structure of the Boost distribution. [#packagers]_

The Structure of a Boost Distribution
=====================================

This is is a sketch of the directory structure you'll get when you
unpack your Boost installation (windows users replace forward
slashes with backslashes): 

.. parsed-literal::

 **boost_1_34_0/** .................\ *The “boost root directory”* 
    **index.html** ....................\ *A copy of www.boost.org*
    **boost/** .........................\ *All Boost Header files*
    **libs/** ............\ *Tests, .cpp*\ s\ *, docs, etc., by library* [#installer-src]_
      **index.html** ........\ *Library documentation starts here*
      **algorithm/**
      **any/**
      **array/**
                      *…more libraries…*
    **status/** .........................\ *Boost-wide test suite*
    **tools/** ...........\ *Utilities, e.g. bjam, quickbook, bcp*
    **more/** ..........................\ *Policy documents, etc.*
    **doc/** ...............\ *A subset of all Boost library docs*

.. sidebar:: Header Organization

  The organization of Boost library headers isn't entirely uniform,
  but most libraries follow a few patterns:

  * Some older libraries and most very small libraries place all
    public headers directly into ``boost/``.

  * Most libraries' public headers live in a subdirectory of
    ``boost/`` named after the library.  For example, you'll find
    the Type Traits Library's ``is_void.hpp`` header in
    ``boost/type_traits/is_void.hpp``.

  * Some libraries have an “aggregate header” in ``boost/`` that
    ``#include``\ s all of the library's other headers.  For
    example, Boost.Python's aggregate header is
    ``boost/python.hpp``.

  * Most libraries place private headers in a subdirectory called
    ``detail/`` or ``aux_/``.  Don't look in these directories and
    expect to find anything you can use.

A few things are worth noting right off the bat:

1. The path to the “boost root directory” is sometimes referred to
   as ``$BOOST_ROOT`` in documentation and mailing lists.  If you
   used the Windows installer, that will usually be |winroot-default|.

2. To compile anything in Boost, you need a directory containing
   the ``boost/`` subdirectory in your ``#include`` path.  For most
   compilers, that means adding

   .. parsed-literal::

      -I\ |root|

   to the command line.  Specific steps for setting up ``#include``
   paths in Microsoft Visual Studio follow later in this document;
   if you use another IDE, please consult your product's
   documentation for instructions.

3. Since all of Boost's header files have the ``.hpp`` extension,
   and live in the ``boost/`` subdirectory of the boost root, your
   Boost ``#include`` directives will look like:

   .. parsed-literal::

     #include <boost/\ *whatever*\ .hpp>

   or

   .. parsed-literal::

     #include "boost/\ *whatever*\ .hpp"

  depending on your religion as regards the use of angle bracket
  includes.  Even Windows users can use forward slashes in
  ``#include`` directives; your compiler doesn't care.

4. Don't be distracted by the ``doc/`` subdirectory; it only
   contains a subset of the Boost documentation.  Start with
   ``libs/index.html`` if you're looking for the whole enchilada.

Building a Simple Boost Program
===============================

The first thing many people want to know is, “how do I build
Boost?”  The good news is that often, there's nothing to build.

.. admonition:: Header-Only Libraries

  Nearly all Boost libraries are **header-only**.  That is, most
  consist entirely of header files containing templates and inline
  functions, and require no separately-compiled library binaries
  or special treatment when linking.  

  The only Boost libraries that are *not* header-only are:

  * Boost.Filesystem
  * Boost.IOStreams
  * Boost.ProgramOptions
  * Boost.Python
  * Boost.Regex
  * Boost.Serialization
  * Boost.Signals
  * Boost.Test
  * Boost.Thread
  * Boost.Wave

  The DateTime library has a separately-compiled
  binary which is only needed if you're using a “legacy
  compiler”(such as?).  The Graph library has a
  separately-compiled binary, but you won't need it unless you
  intend to `parse GraphViz files`__.

__ ../libs/graph/doc/read_graphviz.html

.. ## Keep the list of non-header-only libraries up-to-date


The following program reads a sequence of integers from standard
input, uses Boost.Lambda (a header-only library) to multiply each
one by three, and writes them to standard output::

  #include <boost/lambda/lambda.hpp>
  #include <iostream>
  #include <iterator>
  #include <algorithm>

  int main() 
  {
      using namespace boost::lambda;
      typedef std::istream_iterator<int> in;

      std::for_each( 
          in(std::cin), in(), std::cout << (_1 * 3) << " " );
  }

Start by copying the text of this program into a file called
``example.cpp``.

.. _*nix-header-only:

\*nix (e.g. Unix, Linux, MacOS, Cygwin)
---------------------------------------

Simply issue the following command (``$`` represents the
prompt issued by the shell, so don't type that):

.. parsed-literal::

  **$** c++ -I |root| example.cpp -o example

To test the result, type:

.. parsed-literal::

  **$** echo 1 2 3 | ./example

Microsoft Windows Command-Line using Visual C++
-----------------------------------------------

From your computer's *Start* menu, select if you are a Visual
Studio 2005 user, select

  *All Programs* > *Microsoft Visual Studio 2005* 
  > *Visual Studio Tools* > *Visual Studio 2005 Command Prompt*

or if you're a Visual Studio .NET 2003 user, select

  *All Programs* > *Microsoft Visual Studio .NET 2003* 
  > *Visual Studio .NET Tools* > *Visual Studio .NET 2003 Command Prompt*

to bring up a special command prompt window set up for the Visual
Studio compiler.  In that window, type the following command and
hit the return key (``C:\PROMPT>`` represents the prompt issued by
the shell, so don't type that):

.. parsed-literal::

  **C:\PROMPT>** cl /EHsc /I |winroot| example.cpp

To test the result, type:

.. parsed-literal::

  **C:\PROMPT>** echo 1 2 3 | example

.. _vs-header-only:

Visual Studio .NET 2003 or Visual Studio 2005
---------------------------------------------

* From Visual Studio's *File* menu, select *New* > *Project…*
* In the left-hand pane of the resulting *New Project* dialog,
  select *Visual C++* > *Win32*.
* In the right-hand pane, select *Win32 Console Application*
  (VS8.0) or *Win32 Console Project* (VS7.1).
* In the *name* field, enter “example”
* Right-click **example** in the *Solution Explorer* pane and
  select *Properties* from the resulting pop-up menu
* In *Configuration Properties* > *C/C++* > *General* > *Additional Include
  Directories*, enter the path to the Boost root directory, e.g. 
  |winroot-default|.
* In *Configuration Properties* > *C/C++* > *Precompiled Headers*, change
  *Use Precompiled Header (/Yu)* to *Not Using Precompiled
  Headers*. [#pch]_
* Replace the contents of the ``example.cpp`` generated by the IDE
  with the example code above.
* From the *Build* menu, select *Build Solution*.

To test your application, hit the F5 key and type the following
into the resulting window, followed by the return key::

  1 2 3

Then hold down the control key and press "Z", followed by the
return key.

Other Compilers/Environments
----------------------------

Consult your vendor's documentation; if you have trouble adapting
these instructions to your build environment, request assistance on
the `Boost Users' mailing list`_.

.. _Boost Users' mailing list: mailing_lists.htm#users

Getting Boost Library Binaries
==============================

If you want to use any of the separately-compiled Boost libraries,
you'll need to get ahold of library binaries.  

Microsoft Visual C++ 8.0 or 7.1 (Visual Studio 2005/.NET 2003) Binaries
------------------------------------------------------------------------

The `Windows installer`_ supplied by Boost Consulting will download
and install pre-compiled binaries into the ``lib\`` subdirectory of
the boost root, typically |winroot-default|\ ``\lib\``.

\*nix (e.g. Unix, Linux, MacOS, Cygwin) Binaries
------------------------------------------------

Issue the following commands in the shell (again, ``$`` represents
the shell's prompt):

.. parsed-literal::

  **$** cd |root|
  **$** ./configure --help

Select your configuration options and invoke ``./configure`` again.
Unless you have write permission in your system's ``/usr/local/``
directory, you'll probably want to at least use

.. parsed-literal::

  **$** ./configure **--prefix=**\ *path*\ /\ *to*\ /\ *installation*\ /\ *prefix* 

to install somewhere else.  Finally,

.. parsed-literal::

  **$** make install

which will leave Boost binaries in the ``lib/`` subdirectory of
your installation prefix.  You will also find a copy of the Boost
headers in the ``include/`` subdirectory of the installation
prefix, so you can henceforth use that directory as an ``#include``
path in place of the Boost root directory.

Other Compilers/Environments
----------------------------

If you're not using Visual C++ 7.1 or 8.0, or you're a \*nix user
who wants want to build with a toolset other than your system's
default, or if you want a nonstandard variant build of Boost
(e.g. optimized, but with debug symbols), you'll need to use
Boost.Build_ to create your own binaries.

Building Boost Binaries with Boost.Build_
-----------------------------------------

Like an IDE, Boost.Build_ is a system for developing, testing, and
installing software.  Instead of using a GUI, though, Boost.Build_
is text-based, like ``make``.  Boost.Build_ is written in the
interpreted Boost.Jam_ language.

.. |precompiled-bjam| replace:: pre-compiled ``bjam`` executables

To use Boost.Build_, you'll need an executable called ``bjam``, the
Boost.Jam_ interpreter.  

.. _precompiled-bjam: http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=72941
.. _Boost.Jam documentation: Boost.Jam_
.. _Boost.Build: ../tools/build/index.html
.. _Boost.Jam: ../tools/jam/index.html


.. nosidebar .. sidebar:: Using Boost.Build for your own project

   When you use Boost.Build to build your *own* project, you don't
   need a separate step to create Boost binaries: you simply refer
   to the boost library targets from your Jamfile and the are built
   automatically (refer to the `Boost.Build documentation`_ for
   detailed instructions).  Here, we're assuming you're using a
   different build system for your own code, so you need to
   explicitly generate Boost binaries.  We're also assuming that
   you have a complete Boost distribution somewhere.

.. _Boost.Build documentation: Boost.Build_


Getting ``bjam``
................

.. sidebar:: Using command-line tools in Windows

  In Windows, a command-line tool is invoked by typing its name,
  optionally followed by arguments, into a *Command Prompt* window
  and pressing the Return (or Enter) key.

  To open *Command Prompt*, click the *Start* menu button, click
  *Run*, type “cmd”, and then click OK.

  All commands are executed within the context of a **current
  directory** in the filesystem.  To set the current directory,
  type:

  .. parsed-literal::
  
     cd *path*\ \\\ *to*\ \\\ *some*\ \\\ *directory*

  followed by Return.  For example,

  .. parsed-literal::

     cd |winroot-default|

  One way to name a directory you know about is to write

  .. parsed-literal::

     %HOMEDRIVE%%HOMEPATH%\\\ *directory-name*
     
  which indicates a sibling folder of your “My Documents” folder.

  Long commands can be continued across several lines by typing
  backslashes at the ends of all but the last line.  Many of the
  examples on this page use that technique to save horizontal
  space.

Boost provides |precompiled-bjam|_ for a variety of platforms.
Alternatively, you can build ``bjam`` yourself using the
instructions__ given in the `Boost.Jam documentation`_.

__ http://www.boost.org/doc/html/jam/building.html

``bjam`` is a command-line tool.  To build Boost binaries, you'll
invoke ``bjam`` with the current directory set to the Boost root,
and with options described in the following sections.

.. _toolset:
.. _toolset-name:

Identify Your Toolset
.....................

First, find the toolset corresponding to your compiler in the
following table.  

+-----------+--------------------+-----------------------------+
|Toolset    |Vendor              |Notes                        |
|Name       |                    |                             |
+===========+====================+=============================+
|``acc``    |Hewlett Packard     |Only very recent versions are|
|           |                    |known to work well with Boost|
+-----------+--------------------+-----------------------------+
|``borland``|Borland             |                             |
+-----------+--------------------+-----------------------------+
|``como``   |Comeau Computing    |Using this toolset may       |
|           |                    |require configuring__ another|
|           |                    |toolset to act as its backend|
+-----------+--------------------+-----------------------------+
|``cw``     |Metrowerks/FreeScale|The CodeWarrior compiler.  We|
|           |                    |have not tested versions of  |
|           |                    |this compiler produced since |
|           |                    |it was sold to FreeScale.    |
+-----------+--------------------+-----------------------------+
|``dmc``    |Digital Mars        |As of this Boost release, no |
|           |                    |version of dmc is known to   |
|           |                    |handle Boost well.           |
+-----------+--------------------+-----------------------------+
|``gcc``    |The Gnu Project     |                             |
+-----------+--------------------+-----------------------------+
|``hp_cxx`` |Hewlett Packard     |Targeted at the Tru64        |
|           |                    |operating system.            |
+-----------+--------------------+-----------------------------+
|``intel``  |Intel               |                             |
+-----------+--------------------+-----------------------------+
|``kylix``  |Borland             |                             |
+-----------+--------------------+-----------------------------+
|``msvc``   |Microsoft           |                             |
+-----------+--------------------+-----------------------------+
|``qcc``    |QNX Software Systems|                             |
+-----------+--------------------+-----------------------------+
|``sun``    |Sun                 |                             |
+-----------+--------------------+-----------------------------+
|``vacpp``  |IBM                 |The VisualAge C++ compiler.  |
+-----------+--------------------+-----------------------------+

__ Boost.Build_

If you have multiple versions of a particular compiler installed,
you can apend the version number to the toolset name, preceded by a
hyphen, e.g. ``msvc-7.1`` or ``gcc-3.4``.

.. Note:: if you built ``bjam`` yourself, you may
  have selected a toolset name for that purpose, but that does not
  affect this step in any way; you still need to select a Boost.Build
  toolset from the table.

.. _build directory:
.. _build-directory:

Select a Build Directory
........................

Boost.Build_ will place all intermediate files it generates while
building into the **build directory**.  If your Boost root
directory is writable, this step isn't strictly necessary: by
default Boost.Build will create a ``bin.v2/`` subdirectory for that
purpose in your current working directory.

Invoke ``bjam``
...............

.. |build-directory| replace:: *build-directory*
.. |toolset-name| replace:: *toolset-name*

Change your current directory to the Boost root directory and
invoke ``bjam`` as follows:

.. parsed-literal::

  bjam --build-dir=\ |build-directory|_ \\
       --toolset=\ |toolset-name|_ stage

For example, on Windows, your session might look like:

.. parsed-literal::

   C:\WINDOWS> cd |winroot-default|
   |winroot-default|> bjam \\
     **--build-dir=**\ %HOMEDRIVE%%HOMEPATH%\\build-boost  \\
     **--toolset=msvc stage**

.. Note:: ``bjam`` is case-sensitive; it is important that all the
   parts shown in **bold** type above be entirely lower-case.

And on Unix:

.. parsed-literal::

   ~$ cd ~/|boost_ver|
   ~/|boost_ver|\ $ bjam --build-dir=~/build-boost --prefix=~/boost

In either case, Boost.Build will place the Boost binaries in the
``stage/`` subdirectory of your *build directory*.

``stage``
.........

You already have the Boost headers on your system (in the
``boost/`` subdirectory of your Boost distribution), so if you
prefer not to create an additional copy, instead of installing
Boost you can simply “stage” the Boost binaries, which leaves them
in the ``stage/`` subdirectory of your chosen `build directory`_:

.. parsed-literal::

  bjam --build-dir=\ |build-directory|_ \\
       --toolset=\ |toolset-name|_ stage

.. _prefix directory:
.. _prefix-directory:

Select a Prefix Directory
.........................

Choose a **prefix directory**.  The installation process will
leave you with the following subdirectories of the prefix directory:

* ``lib``, containing the Boost binaries
* ``include/``\ |boost_ver|, containing the Boost headers.

.. |prefix-directory| replace:: *prefix-directory*

Change your current directory to the Boost root directory and
invoke ``bjam`` as follows:

.. parsed-literal::

  bjam --build-dir=\ |build-directory|_ \\
       --toolset=\ |toolset-name|_      \\
        --prefix=\ |prefix-directory|_ install

For example, on Windows your session might look like:

.. parsed-literal::

   C:\WINDOWS> cd |winroot-default|
   |winroot-default|> bjam \\
       --build-dir=C:\\TEMP\\build-boost       \\
       --prefix=C:\\boost

And on Unix:

.. parsed-literal::

   ~$ cd ~/|boost_ver|
   ~/|boost_ver|\ $ bjam --build-dir=/tmp/build-boost \\
        --prefix=~/boost

Linking A Program with a Boost Library
======================================

To demonstrate linking with a Boost binary library, we'll use the
following simple program that extracts the subject lines from
emails.  It uses the Boost.Regex_ library, which has a
separately-compiled binary component. ::

  #include <boost/regex.hpp>
  #include <iostream>
  #include <string>

  int main()
  {
      std::string line;
      boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

      while (std::cin)
      {
          std::getline(std::cin, line);
          boost::smatch matches;
          if (boost::regex_match(line, matches, pat))
              std::cout << matches[2];
      }
  }

.. _Boost.Regex: ../libs/regex

There are two main challenges associated with linking:

1. Tool configuration, e.g. choosing command-line options or IDE
   build settings.

2. Identifying the library binary, among all the build variants,
   whose compile configuration is compatible with the rest of your
   project.

Microsoft Windows
-----------------

Most Windows compilers and linkers have so called “auto-linking
support,” which is used by many Boost libraries to eliminate the
second challenge.  Special code in Boost header files detects your
compiler options and uses that information to encode the name of
the correct library into your object files; the linker selects the
library with that name from the directories you've told it to
search.

.. Note:: As of this writing, a few Boost libraries don't support
   auto-linking:

   * Boost.Python
   * …others?…

Visual C++ Command Line
.......................

For example, we can compile and link the above program from the
Visual C++ command-line by simply adding the **bold** text below to
the command line we used earlier, assuming your Boost binaries are
in |winroot-default|:

.. parsed-literal::

   C:\PROMPT> cl /EHsc /I |winroot| example.cpp **\\**
        **/link /LIBPATH:** |bold-winroot-default|

To link with a library that doesn't use auto-linking support, you
need to specify the library name.  For example,

.. parsed-literal::

   C:\PROMPT> cl /EHsc /I |winroot| example.cpp \\
        /link /LIBPATH: |winroot-default| **\\**
        **boost_regex-msvc-7.1-mt-d-1_34.lib**

See `Library Naming`_ for details about how to select the right
library name.

Visual Studio IDE
.................

Starting with the `header-only example project`__ we created
earlier:

__ vs-header-only_

1. Right-click **example** in the *Solution Explorer* pane and
   select *Properties* from the resulting pop-up menu
2. In *Configuration Properties* > *Linker* > *Additional Library
   Directories*, enter the path to the Boost binaries,
   e.g. |winroot-default|\ ``\lib\``.
3. From the *Build* menu, select *Build Solution*.

To link with a library that doesn't use auto-linking support,
before building (step 3 above), you also need to specify the library
name:

* In *Configuration Properties* > *Linker* > *Input* >
  *Additional Dependencies*, enter the name of the binary library
  to link with, e.g. **boost_regex-msvc-7.1-mt-d-1_34.lib**.

See `Library Naming`_ for details about how to select the right
library name.

\*nix (e.g. Unix, Linux, MacOS, Cygwin)
---------------------------------------

There are two main ways to link to libraries:

a. You can specify the full path to each library:

   .. parsed-literal::

     $ c++ -I |root| example.cpp -o example **\\**
        **~/boost/lib/libboost_regex-msvc-7.1-mt-d-1_34.a**

b. You can separately specify a directory to search (with ``-L``\
   *directory*) and a library name to search for (with ``-l``\
   *library*, [#lowercase-l]_ dropping the filename's leading ``lib`` and trailing
   suffix (``.a`` in this case): 

   .. parsed-literal::

     $ c++ -I |root| example.cpp -o example **\\**
        **-L~/boost/lib/ -lboost_regex-msvc-7.1-mt-d-1_34**

   As you can see, this method is just as terse as method a. for
   one library; it *really* pays off when you're using multiple
   libraries from the same directory.

In both cases above, the bold text is what you'd add to `the
command lines we explored earlier`__.

__ *nix-header-only_

Library Naming
--------------

In order to choose the right library binary to link with, you'll
need to know something about how Boost libraries are named.  Each
library binary filename is composed of a common sequence of
elements that describe how it was built.  For example,
``libboost_regex-msvc-7.1-mt-d-1_34.lib`` can be broken down into the
following elements:

``lib`` 
  *Prefix*: except on Microsoft Windows, every Boost library
  name begins with this string.  On Windows, only ordinary static
  libraries use the ``lib`` prefix; import libraries and DLLs do
  not. [#distinct]_

``boost_regex``
  *Library name*: all boost library filenames begin with ``boost_``.

``-msvc-7.1``
   *Toolset tag*: one of the `Boost.Build toolset names`_,
   possibly followed by a dash and a version number.

``-mt``
   *Threading tag*: indicates that the library was
   built with multithreading support enabled.  Libraries built
   without multithreading support can be identified by the absence
   of ``-mt``.

``-d``
   *ABI tag*: encodes details that affect the library's
   interoperability with other compiled code.  For each such
   feature, a single letter is added to the tag:

   +-----+------------------------------------------------------------------------------+
   |Key  |Use this library when:                                                        |
   +=====+==============================================================================+
   |``s``|linking statically to the C++ standard library and compiler runtime support   |
   |     |libraries.                                                                    |
   +-----+------------------------------------------------------------------------------+
   |``g``|using debug versions of the standard and runtime support libraries.           |
   +-----+------------------------------------------------------------------------------+
   |``y``|using a special `debug build of Python`__.                                    |
   +-----+------------------------------------------------------------------------------+
   |``d``|building a debug version of your code. [#debug-abi]_                          |
   +-----+------------------------------------------------------------------------------+
   |``p``|using the STLPort standard library rather than the default one supplied with  |
   |     |your compiler.                                                                |
   +-----+------------------------------------------------------------------------------+
   |``n``|using STLPort's deprecated “native iostreams” feature. [#native]_             |
   +-----+------------------------------------------------------------------------------+

   For example, if you build a debug version of your code for use
   with debug versions of the static runtime library and the
   STLPort standard library in “native iostreams” mode,
   the tag would be: ``-sgdpn``.  If none of the above apply, the
   ABI tag is ommitted.

``-1_34``
  *Version tag*: the full Boost release number,
  with periods replaced by underscores. The major and minor version
  numbers are taken together separated by an underscore. For
  example, version 1.31.1 would be tagged as "-1_31_1". 

``.lib``
  *Extension*: determined according to the
  operating system's usual convention.  On Windows, ``.dll``
  indicates a shared library and ``.lib`` indicates a static or
  import library.  On most \*nix platforms the extensions are
  ``.a`` and ``.so`` for static libraries (archives) and shared
  libraries, respectively.  Where supported by \*nix toolsets, a
  full version extension is added (e.g. ".so.1.34"); a symbolic
  link to the library file, named without the trailing version
  number, will also be created.

.. _Boost.Build toolset names: toolset-name_

__ ../libs/python/doc/building.html#variants


------------------------------

.. [#packagers] If developers of Boost packages would like to work
   with us to make sure these instructions can be used with their
   packages, we'd be glad to help.  Please make your interest known
   to the `Boost developers' list`_.

.. [#installer-src] If you used the `Windows installer`_ from Boost
   Consulting and deselected “Source and Documentation”  (it's
   selected by default), you won't see the ``libs/`` subdirectory.
   That won't affect your ability to use precompiled binaries, but
   you won't be able to rebuild libraries from scratch.

.. _Boost developers' list: mailing_lists.htm#main

.. [#pch] There's no problem using Boost with precompiled headers;
   these instructions merely avoid precompiled headers because it
   would require Visual Studio-specific changes to the source code
   used in the examples.

.. [#lowercase-l] That option is a dash followed by a lowercase “L”
   character, which looks very much like a numeral 1 in some fonts.

.. [#distinct] This convention distinguishes the static version of
   a Boost library from the import library for an
   identically-configured Boost DLL, which would otherwise have the
   same name.

.. [#debug-abi] These libraries were compiled without optimization
   or inlining, with full debug symbols enabled, and without
   ``NDEBUG`` ``#define``\ d.  All though it's true that sometimes
   these choices don't affect binary compatibility with other
   compiled code, you can't count on that with Boost libraries.

.. [#native] This feature of STLPort is deprecated because it's
   impossible to make it work transparently to the user; we don't
   recommend it.

