+++
title = "Build and install"
weight = 3
+++

## Usage as a single header file

Outcome v2 comes in single header file form. This is regenerated per commit. To fetch
on Linux:

```
wget https://github.com/ned14/outcome/raw/master/single-header/outcome.hpp
```

On BSD:

```
fetch https://github.com/ned14/outcome/raw/master/single-header/outcome.hpp
```

If you have `curl` installed:

```
curl -O -J -L https://github.com/ned14/outcome/raw/master/single-header/outcome.hpp
```

Otherwise, simply download the raw file from above and place it wherever it suits you.
If you might be debugging using Microsoft Visual Studio, you may find the debugger
visualisation file at https://github.com/ned14/outcome/raw/master/include/outcome/outcome.natvis
useful to include into your build.


## Usage from the vcpkg package manager

This is particularly easy, and works on Mac OS, Linux and Microsoft Windows:

```
vcpkg install outcome
```

Outcome appears at `<boost/outcome/outcome.hpp>`. This is a full copy of Outcome, so
Experimental Outcome and all the usual headers are where you would expect.


## Usage from the Conan package manager

*(thanks to Théo Delrieu for contributing this support)*

At the command line, add the bintray repo for Outcome to conan:

```
conan remote add outcome https://api.bintray.com/conan/ned14/Outcome
```

Now simply add this to your Conan build:

```
[requires]
Outcome/master@ned14/stable
```

Outcome will be made available by Conan at `<outcome.hpp>`.


## Usage from the cmake hunter package manager

Outcome has not been submitted to the main cmake hunter package manager repo yet.
You can however add it as a git submodule:

```
cd yourthirdpartyrepodir
git submodule add https://github.com/ned14/quickcpplib
git submodule add https://github.com/ned14/outcome
cd ..
git submodule update --init --recursive
```

Now tell cmake hunter about a git submoduled cmake hunter package by
adding to your project's `cmake/Hunter/config.cmake`:

```
hunter_config(quickcpplib GIT_SUBMODULE "yourthirdpartyrepodir/quickcpplib")
hunter_config(outcome GIT_SUBMODULE "yourthirdpartyrepodir/outcome")
```

... and finally to your `CMakeLists.txt`, now add outcome as if it were
an ordinary cmake hunter package:

```
hunter_add_package(quickcpplib)
find_package(quickcpplib CONFIG REQUIRED)
hunter_add_package(outcome)
find_package(outcome CONFIG REQUIRED)
```

Now you tell cmake to link to outcome as usual (see below for cmake targets):

```
target_link_libraries(mytarget outcome::hl)
```

## Usage as a git submodule

If you are very keen on tracking very latest Outcome, you can add it as a git
submodule to your project so you can keep abreast of bug fixes. Here is how:

```
git submodule add https://github.com/ned14/outcome
cd outcome
git checkout master
git submodule update --init --recursive
```

After this you can bring Outcome into your code using:

```
#include "outcome/single-header/outcome.hpp"
```

That's it, you are ready to go. From time to time, you may wish to update to
latest:

```
cd outcome
git pull
git submodule update
```

## Usage as a stable source tarball

If you would prefer a single source tarball of the stable and develop branches
known to have had all unit tests passing on all platforms, containing all the
documentation, tests and sources, this can always be retrieved from:

https://github.com/ned14/outcome/releases

This tarball is automatically generated when Outcome fully compiles and passes
all unit tests on all platforms tested by the CIs. This currently includes:

- Linux: GCC 7.5, clang 9, clang 11
- MacOS: XCode 12
- Windows: VS2019.7

All unit tests are executed under the Address and Undefined Behaviour sanitisers.

It should be emphasised that newer compilers are not tested, so there is
an unlikely chance that the tarball may not work on a newer compiler.

<hr>

# Running the unit test suite

To run the unit test suite you will need cmake 3.3 or later installed.

```
mkdir build
cd build
cmake ..
cmake --build .
ctest
```

On some cmake generators (Visual Studio, Xcode) you may need to tell cmake build a configuration
like Release or Debug. Similarly, ctest needs to be told the same e.g.

```
mkdir build
cd build
cmake ..
cmake --build . --config Release
ctest -C Release
```

[Per commit, tests are run by Travis and uploaded to a CDash dashboard here](http://my.cdash.org/index.php?project=Boost.Outcome).

<hr>

# CMake `find_package()` imported targets support

Outcome fully complies with cmake install, so by installing Outcome, it can be
found by cmake's `find_package()`.

```
mkdir build
cd build
cmake ..
cmake --build .
sudo cmake --build . --target install
```

# Modular CMake build support

If you are using Outcome in a CMake project, Outcome is a "modular cmake" project
using only modern cmake 3 throughout. This lets you add the Outcome directory as a
cmake subdirectory with no unexpected consequence on the rest of your cmake. You will need
to be using cmake 3.3 or better.

```
add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/outcome"  # path to outcome source
  "${CMAKE_CURRENT_BINARY_DIR}/outcome"  # your choice of where to put binaries
  EXCLUDE_FROM_ALL                       # please only lazy build outcome on demand
)
```

Outcome's cmake has the following useful products:

- `outcome::hl` (target): the Outcome header-only library. Add this to any
`target_link_libraries()` in your cmake to bring in Outcome as a header-only library. This will also
add to your link (via `PUBLIC`) any debugger visualisation support files, any system library
dependencies and also force all consuming executables to be configured with a minimum
of C++ 14 as Outcome requires a minimum of that.
- `outcome_TEST_TARGETS` (list): a list of targets which generate Outcome's test
suite. You can append this to your own test suite if you wish to run Outcome's test
suite along with your own.
