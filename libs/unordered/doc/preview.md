# Building Boost from the Tip of Develop

To build Boost from the tip of the develop branch without cloning the entire 
history, use the command below:

Linux:
```bash
cwd=$(pwd) \
  && cd ~ \
  && git clone -b develop --depth 1 --recurse-submodules --shallow-submodules --jobs 8 https://github.com/boostorg/boost.git boost-develop-beta \
  && cd boost-develop-beta \
  && ./bootstrap.sh \
  && ./b2 install --prefix=boost-install cxxstd=17 \
  && echo "Boost successfully installed into $(realpath boost-install)!
  Add this to your CMake toolchain file.
  list(APPEND CMAKE_PREFIX_PATH $(realpath boost-install))
  " \
  && cd $cwd
```

Windows (using plain Command Prompt):
```bat
cmd /v
set cwd=%cd% ^
  && cd %homepath% ^
  && git clone -b develop --depth 1 --recurse-submodules --shallow-submodules --jobs 8 https://github.com/boostorg/boost.git boost-develop-beta ^
  && cd boost-develop-beta ^
  && bootstrap.bat ^
  && b2 install --prefix=boost-install cxxstd=17 address-model=64 ^
  && echo Boost successfully installed into !cd!\boost-install! ^
  && echo Add this to your CMake toolchain file. ^
  && echo list(APPEND CMAKE_PREFIX_PATH !cd:\=/!/boost-install) ^
  && cd !cwd! ^
  && exit
```

Note: you can build Boost with a specific compiler by setting the toolset in 
the `./b2` command above. To build with clang, specify `toolset=clang`; to build
with a specific version of gcc, clang or msvc, specify e.g. `toolset=gcc-12` for GCC 
12, `clang-14` for Clang 14, `msvc-14.3` for MSVC 14.3. The value of `cxxstd` 
can also be set to other values such as 11 for C++11, 14 for C++14, etc.

For more info on what's possible, check out this link on b2:
https://www.boost.org/doc/libs/develop/tools/build/doc/html/index.html#bbv2.overview.builtins.features

This should hopefully cover the two most common cases of building a dependency, 
setting the compiler and C++ standard used.
