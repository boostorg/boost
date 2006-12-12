============================
 |(logo)|__ Getting Started
============================

.. |(logo)| image:: ../boost.png
   :alt: Boost
   :class: boost-logo

__ ../index.htm


.. section-numbering::
   :depth: 2

.. contents:: Contents
   :depth: 2
   :class: sidebar small

.. ## Update this substitution for each release

.. |boost_ver| replace:: ``boost_1_34_0``
.. |boost_ver-bold| replace:: **boost_1_34_0**

.. |root| replace:: ``/``\ *path*\ ``/``\ *to*\ ``/``\ |boost_ver|
.. |winroot| replace:: *path*\ ``\``\ *to*\ ``\``\ |boost_ver|
.. |winroot-default| replace:: ``C:\Program``\ `` ``\ ``Files\boost\``\ |boost_ver|
.. |bold-winroot-default| replace:: **C:\\Program Files\\boost\\**\ |boost_ver-bold|

Introduction
============

Welcome to the Boost libraries!  By the time you've completed this
tutorial, you'll be at least somewhat comfortable with the contents
of a Boost distribution and how to go about using it.  

What's Here
-----------

This document is designed to be an *extremely* gentle introduction,
so we included a fair amount of material that may already be very
familiar to you.  To keep things simple, we also left out some
information intermediate and advanced users will probably want.  At
the end of this document, we'll refer you on to resources that can
help you pursue these topics further.

Preliminaries
-------------

We use one typographic convention that might not be immediately
obvious: *italic* text in examples is meant as a descriptive
placeholder for something else, usually information that you'll
provide.  For example:

.. parsed-literal::

   **$** echo "My name is *your name*\ "

Here you're expected to imagine replacing the text “your name” with
your actual name.

We identify Unix and its variants such as Linux, FreeBSD, and MacOS
collectively as \*nix.  If you're not targeting Microsoft Windows,
the instructions for \*nix users will probably work for you.
Cygwin users working from the Cygwin ``bash`` prompt should also
follow the \*nix instructions.  To use your Cygwin compiler from
the Windows command prompt, follow the instructions for Windows
users.

Although Boost supports a wide variety of Windows compilers
(including older Microsoft compilers), most instructions for
Windows users cover only the Visual Studio .NET 2003 and Visual
Studio 2005.  We hope that gives you enough information to adapt
them for your own compiler or IDE.

Get Boost
=========

There are basically three ways to get Boost on your system:

1. **Windows Installer**: Boost Consulting provides an installer_
   for Windows platforms that installs a complete Boost
   distribution, plus optional precompiled library binaries for
   Visual Studio, and (optionally) a prebuilt version of the
   ``bjam`` build tool.

   .. _Windows installer: http://www.boost-consulting.com/download.html
   .. |Windows installer| replace:: **Windows installer**
   .. _Boost Consulting: http://boost-consulting.com
   .. _installer: `Windows installer`_


2. **Download**: users of other platforms—and Windows
   users who prefer to build everything from scratch—can `download
   a complete Boost distribution`__ from SourceForge.

   .. ## Update this link for each release
   __ http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=8041&release_id=376197

   - **Windows**: Download and run |boost_ver|\ ``.exe`` 
     to unpack the distribution. [#zip]_

   - ***nix**: Download |boost_ver|\ ``.tar.bz2``.  Then, in the
     directory where you want to put the Boost installation,
     execute

     .. parsed-literal::
   
        tar --bzip2 -xf */path/to/*\ |boost_ver|\ .tar.bz2

3. **Boost packages** from RedHat, Debian, or some other
   distribution packager: these instructions may not work for you
   if you use 3rd party packages, because other packagers sometimes
   choose to break Boost up into several packages or to reorganize
   the directory structure of the Boost distribution. [#packagers]_

The Structure of a Boost Distribution
=====================================

This is is a sketch of the directory structure you'll get when you
unpack your Boost installation (windows users replace forward
slashes with backslashes): 

.. parsed-literal::

 |boost_ver-bold|\ **/** .................\ *The “boost root directory”* 
    **index.htm** .........\ *A copy of www.boost.org starts here*
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
  :class: small

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
    example, Boost.Python_'s aggregate header is
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

Header-Only Libraries
=====================

The first thing many people want to know is, “how do I build
Boost?”  The good news is that often, there's nothing to build.

.. admonition:: Nothing to Build

  Most Boost libraries are **header-only**: they consist *entirely
  of header files* containing templates and inline functions, and
  require no separately-compiled library binaries or special
  treatment when linking.

.. _separate: 

The only Boost libraries that can't be used without separate
compilation are:

* Boost.Filesystem
* Boost.IOStreams
* Boost.ProgramOptions
* Boost.Python_
* Boost.Regex
* Boost.Serialization
* Boost.Signals
* Boost.Test
* Boost.Thread
* Boost.Wave

The DateTime library has a separately-compiled component that
is only needed if you're using its to/from_string and/or
serialization features or if you're targeting Visual C++ 6.x or
Borland.  The Graph library also has a separately-compiled part,
but you won't need it unless you intend to `parse GraphViz
files`__.

__ ../libs/graph/doc/read_graphviz.html

.. ## Keep the list of non-header-only libraries up-to-date

Build a Simple Program Using Boost
==================================

To keep things simple, let's start by using a header-only library.
The following program reads a sequence of integers from standard
input, uses Boost.Lambda to multiply each number by three, and
writes them to standard output::

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

Copy the text of this program into a file called ``example.cpp``.

.. _unix-header-only:

Build on \*nix
--------------

In the directory where you saved ``example.cpp``, issue the
following command:

.. parsed-literal::

  c++ -I |root| example.cpp -o example

To test the result, type:

.. parsed-literal::

  echo 1 2 3 | ./example

.. |next| replace:: *next...*

|next|__

__ `Errors and Warnings`_

Build from the Visual Studio Command Prompt
-------------------------------------------

From your computer's *Start* menu, if you are a Visual
Studio 2005 user, select

  *All Programs* > *Microsoft Visual Studio 2005* 
  > *Visual Studio Tools* > *Visual Studio 2005 Command Prompt*

or, if you're a Visual Studio .NET 2003 user, select

  *All Programs* > *Microsoft Visual Studio .NET 2003* 
  > *Visual Studio .NET Tools* > *Visual Studio .NET 2003 Command Prompt*

to bring up a special `command prompt`_ window set up for the Visual
Studio compiler.  In that window, type the following command and
hit the return key:

.. parsed-literal::

  cl /EHsc /I\ |winroot| *path*\ \\\ *to*\ \\example.cpp

To test the result, type:

.. parsed-literal::

  echo 1 2 3 | example

|next|__

__ `Errors and Warnings`_

.. _vs-header-only:

Build in the Visual Studio IDE
------------------------------

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

Errors and Warnings
-------------------

Don't be alarmed if you see compiler warnings from Boost headers.
We try to eliminate them, but doing so isn't always practical.
[#warnings]_

Errors are another matter.  If you're seeing compilation errors at
this point in the tutorial, check to be sure you've copied the
example program correctly and that you've correctly identified the
Boost root directory.

Get Boost Library Binaries
==========================

If you want to use any of the separately-compiled Boost libraries,
you'll need library binaries.

Install Visual Studio Binaries
------------------------------

The `Windows installer`_ supplied by Boost Consulting will download
and install pre-compiled binaries into the ``lib\`` subdirectory of
the boost root, typically |winroot-default|\ ``\lib\``.

|next|__

__ `Link Your Program to a Boost Library`_

Build and Install \*nix Binaries
--------------------------------

Issue the following commands in the shell (don't type ``$``; it
represents the shell's prompt):

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

|next|__

__ `Expected Build Output`_

Build and Install Other Binaries
--------------------------------

If you're not using Visual C++ 7.1 or 8.0, or you're a \*nix user
who wants want to build with a toolset other than your system's
default, or if you want a nonstandard variant build of Boost
(e.g. optimized, but with debug symbols), you'll need to use
Boost.Build_ to create your own binaries.

Boost.Build_ is a text-based system for developing, testing, and
installing software.  To use it, you'll need an executable called
``bjam``.

.. |precompiled-bjam| replace:: pre-compiled ``bjam`` executables


.. _precompiled-bjam: http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=72941
.. _Boost.Jam documentation: Boost.Jam_
.. _Boost.Build: ../tools/build/index.html
.. _Boost.Jam: ../tools/jam/index.html
.. _Boost.Build documentation: Boost.Build_

Get ``bjam``
............

``bjam`` is the `command-line tool`_ that drives the Boost Build
system.  To build Boost binaries, you'll invoke ``bjam`` from the
Boost root.  

Boost provides |precompiled-bjam|_ for a variety of platforms.
Alternatively, you can build ``bjam`` yourself using `these
instructions`__.

__ http://www.boost.org/doc/html/jam/building.html


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
|``darwin`` |Apple Computer      |Apple's version of the GCC   |
|           |                    |toolchain with support for   |
|           |                    |Darwin and MacOS X features  |
|           |                    |such as frameworks.          |
+-----------+--------------------+-----------------------------+
|``gcc``    |The Gnu Project     |Includes support for Cygwin  |
|           |                    |and MinGW compilers.         |
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
|``sun``    |Sun                 |Only very recent versions are|
|           |                    |known to work well with      |
|           |                    |Boost.                       |
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

  bjam **--build-dir=**\ |build-directory|_ **\\**
       **--toolset=**\ |toolset-name|_ stage

For example, on Windows, your session might look like:

.. parsed-literal::

   C:\WINDOWS> cd |winroot-default|
   |winroot-default|> bjam **\\**
     **--build-dir=**\ %TEMP%\\build-boost          **\\**
     **--toolset=msvc stage**

And on Unix:

.. parsed-literal::

   $ cd ~/|boost_ver|
   $ bjam **--build-dir=**\ ~/build-boost **--prefix=**\ ~/boost

In either case, Boost.Build will place the Boost binaries in the
``stage/`` subdirectory of your `build directory`_.

.. Note:: ``bjam`` is case-sensitive; it is important that all the
   parts shown in **bold** type above be entirely lower-case.

For a description of other options you can pass when invoking
``bjam``, type::

  bjam --help

Expected Build Output
---------------------

During the process of building Boost libraries, you can expect to
see some messages printed on the console.  These may include

* Notices about Boost library configuration—for example, the Regex
  library outputs a message about ICU when built without Unicode
  support, and the Python library may be skipped without error (but
  with a notice) if you don't have Python installed.

* Messages from the build tool that report the number of targets
  that were built or skipped.  Don't be surprised if those numbers
  don't make any sense to you; there are many targets per library.

* Build action messages describing what the tool is doing, which
  look something like:

  .. parsed-literal::

    *toolset-name*.c++ *long*\ /\ *path*\ /\ *to*\ /\ *file*\ /\ *being*\ /\ *built*

* Compiler warnings.

In Case of Build Errors
-----------------------

The only error messages you see when building Boost—if any—should
be related to the IOStreams library's support of zip and bzip2
formats as described here__.  Install the relevant development
packages for libz and libbz2 if you need those features.  Other
errors when building Boost libraries are cause for concern.

If it seems like the build system can't find your compiler and/or
linker, consider setting up a ``user-config.jam`` file as described
in the `Boost.Build documentation`_.  If that isn't your problem or
the ``user-config.jam`` file doesn't work for you, please address
questions about configuring Boost for your compiler to the
`Boost.Build mailing list`_.

__ file:///home/dave/src/boost/libs/iostreams/doc/installation.html

Link Your Program to a Boost Library
====================================

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
              std::cout << matches[2] << std::endl;
      }
  }

.. _Boost.Regex: ../libs/regex

There are two main challenges associated with linking:

1. Tool configuration, e.g. choosing command-line options or IDE
   build settings.

2. Identifying the library binary, among all the build variants,
   whose compile configuration is compatible with the rest of your
   project.

.. Note:: Boost.Python_ users should read that library's own `build
   documentation`__ as there are several library-specific issues to
   consider.

.. _Boost.Python: ../libs/python/index.html
__ ../libs/python/doc/building.html

Link to a Boost Library on Windows
----------------------------------

.. _auto-linking:

Most Windows compilers and linkers have so-called “auto-linking
support,” which eliminates the second challenge.  Special code in
Boost header files detects your compiler options and uses that
information to encode the name of the correct library into your
object files; the linker selects the library with that name from
the directories you've told it to search.

Link to a Boost Library from the Visual Studio Command Prompt
.............................................................

For example, we can compile and link the above program from the
Visual C++ command-line by simply adding the **bold** text below to
the command line we used earlier, assuming your Boost binaries are
in |winroot-default|\ ``\lib``:

.. parsed-literal::

   cl /EHsc /I |winroot| example.cpp   **\\**
        **/link /LIBPATH:** |bold-winroot-default|\ **\\lib**

|next|__

__ `Test Your Program`_

Link to a Boost Library in the Visual Studio IDE
................................................

Starting with the `header-only example project`__ we created
earlier:

__ vs-header-only_

1. Right-click **example** in the *Solution Explorer* pane and
   select *Properties* from the resulting pop-up menu
2. In *Configuration Properties* > *Linker* > *Additional Library
   Directories*, enter the path to the Boost binaries,
   e.g. |winroot-default|\ ``\lib\``.
3. From the *Build* menu, select *Build Solution*.

|next|__

__ `Test Your Program`_

Link to a Boost Library On \*nix
--------------------------------

There are two main ways to link to libraries:

A. You can specify the full path to each library:

   .. parsed-literal::

     $ c++ -I |root| example.cpp -o example **\\**
        **~/boost/lib/libboost_regex-gcc-3.4-mt-d-1_34.a**

B. You can separately specify a directory to search (with ``-L``\
   *directory*) and a library name to search for (with ``-l``\
   *library*, [#lowercase-l]_ dropping the filename's leading ``lib`` and trailing
   suffix (``.a`` in this case): 

   .. parsed-literal::

     $ c++ -I |root| example.cpp -o example **\\**
        **-L~/boost/lib/ -lboost_regex-gcc-3.4-mt-d-1_34**

   As you can see, this method is just as terse as method A for one
   library; it *really* pays off when you're using multiple
   libraries from the same directory.  Note, however, that if you
   use this method with a library that has both static (``.a``) and
   dynamic (``.so``) builds, the system may choose one
   automatically for you unless you pass a special option such as
   ``-static`` on the command line.

In both cases above, the bold text is what you'd add to `the
command lines we explored earlier`__.

__ unix-header-only_

Library Naming
--------------

When auto-linking is not available, you need to know how Boost
binaries are named so you can choose the right one for your build
configuration.  Each library filename is composed of a common
sequence of elements that describe how it was built.  For example,
``libboost_regex-vc71-mt-d-1_34.lib`` can be broken down into the
following elements:

``lib`` 
  *Prefix*: except on Microsoft Windows, every Boost library
  name begins with this string.  On Windows, only ordinary static
  libraries use the ``lib`` prefix; import libraries and DLLs do
  not. [#distinct]_

``boost_regex``
  *Library name*: all boost library filenames begin with ``boost_``.

``-vc71``
   *Toolset tag*: identifies the toolset and version used to build
   the binary.

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
  *Version tag*: the full Boost release number, with periods
  replaced by underscores. For example, version 1.31.1 would be
  tagged as "-1_31_1".

``.lib``
  *Extension*: determined according to the operating system's usual
  convention.  On most \*nix platforms the extensions are ``.a``
  and ``.so`` for static libraries (archives) and shared libraries,
  respectively.  On Windows, ``.dll`` indicates a shared library
  and—except for static libraries built by ``gcc`` toolset, whose
  names always end in ``.a``— ``.lib`` indicates a static or import
  library.  Where supported by \*nix toolsets, a full version
  extension is added (e.g. ".so.1.34") and a symbolic link to the
  library file, named without the trailing version number, will
  also be created.

.. _Boost.Build toolset names: toolset-name_

__ ../libs/python/doc/building.html#variants

Test Your Program
-----------------

To test our subject extraction, we'll filter the following text
file.  Copy it out of your browser and save it as ``jayne.txt``::

   To: George Shmidlap
   From: Rita Marlowe
   Subject: Will Success Spoil Rock Hunter?
   ---
   See subject.

Test Your Program on Microsoft Windows
......................................

In a `command prompt`_ window, type:

.. parsed-literal::

   *path*\ \\\ *to*\ \\\ *compiled*\ \\example < *path*\ \\\ *to*\ \\\ jayne.txt

The program should respond with the email subject, “Will Success
Spoil Rock Hunter?”

Test Your Program on \*nix
..........................

If you linked to a shared library, you may need to prepare some
platform-specific settings so that the system will be able to find
and load it when your program is run.  Most platforms have an
environment variable to which you can add the directory containing
the library.  On many platforms (Linux, FreeBSD) that variable is
``LD_LIBRARY_PATH``, but on MacOS it's ``DYLD_LIBRARY_PATH``, and
on Cygwin it's simply ``PATH``.  In most shells other than ``csh``
and ``tcsh``, you can adjust the variable as follows (again, don't
type the ``$``\ —that represents the shell prompt):

.. parsed-literal::

   **$** *VARIABLE_NAME*\ =\ *path/to/lib/directory*\ :${\ *VARIABLE_NAME*\ }
   **$** export *VARIABLE_NAME*

On ``csh`` and ``tcsh``, it's

.. parsed-literal::

   **$** setenv *VARIABLE_NAME* *path/to/lib/directory*\ :${\ *VARIABLE_NAME*\ }

Once the necessary variable (if any) is set, you can run your
program as follows:

.. parsed-literal::

   **$** *path*\ /\ *to*\ /\ *compiled*\ /\ example < *path*\ /\ *to*\ /\ jayne.txt

The program should respond with the email subject, “Will Success
Spoil Rock Hunter?”

Conclusion and Further Resources
================================

This concludes your introduction to Boost and to integrating it
with your programs.  As you start using Boost in earnest, there are
surely a few additional points you'll wish we had covered.  One day
we may have a “Book 2 in the Getting Started series” that addresses
them.  Until then, we suggest you pursue the following resources.
If you can't find what you need, or there's anything we can do to
make this document clearer, please post it to the `Boost Users'
mailing list`_.

* `Boost.Build reference manual`_
* `Boost.Jam reference manual`_
* `Boost Users' mailing list`_
* `Boost.Build mailing list`_
* `Boost.Build Wiki`_

.. Admonition:: Onward

  .. epigraph::

     Good luck, and have fun!

     -- the Boost Developers

.. _Boost.Build reference manual: ../tools/build/v2
.. _Boost.Jam reference manual: `Boost.Jam`_
.. _Boost Users' mailing list: mailing_lists.htm#users
.. _Boost.Build Wiki: http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?Boost.Build_V2
.. _Boost.Build mailing list: mailing_lists.htm#jamboost


.. _`Using command-line tools in Windows`:
.. _`command prompt`:
.. _`command-line tool`:

Appendix: Using command-line tools in Windows
=============================================

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

------------------------------

.. [#zip] If you prefer not to download executable programs, download
   |boost_ver|\ ``.zip`` and use an external tool to decompress
   it.  We don't recommend using Windows' built-in decompression as
   it can be painfully slow for large archives.

.. [#packagers] If developers of Boost packages would like to work
   with us to make sure these instructions can be used with their
   packages, we'd be glad to help.  Please make your interest known
   to the `Boost developers' list`_.

.. _Boost developers' list: mailing_lists.htm#main

.. [#installer-src] If you used the `Windows installer`_ from Boost
   Consulting and deselected “Source and Documentation”  (it's
   selected by default), you won't see the ``libs/`` subdirectory.
   That won't affect your ability to use precompiled binaries, but
   you won't be able to rebuild libraries from scratch.

.. [#warnings] Remember that warnings are specific to each compiler
   implementation.  The developer of a given Boost library might
   not have access to your compiler.  Also, some warnings are
   extremely difficult to eliminate in generic code, to the point
   where it's not worth the trouble.  Finally, some compilers don't
   have any source code mechanism for suppressing warnings.

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

