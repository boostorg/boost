# Boost.Unordered

Part of collection of the [Boost C++ Libraries](http://github.com/boostorg).

For accessing data based on key lookup, the C++ standard library offers `std::set`, `std::map`, `std::multiset` and `std::multimap`.
These are generally implemented using balanced binary trees so that lookup time has logarithmic complexity.
That is generally okay, but in many cases a hash table can perform better, as accessing data has constant complexity, on average.
The worst case complexity is linear, but that occurs rarely and with some care, can be avoided.

Also, the existing containers require a 'less than' comparison object to order their elements.
For some data types this is impossible to implement or isn’t practical.
In contrast, a hash table only needs an equality function and a hash function for the key.

With this in mind, unordered associative containers were added to the C++ standard.
This is an implementation of the containers described in C++11, with some deviations from the standard in order to work with non-C++11 compilers and libraries.


### License

Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

### Properties

* C++03
* Header-Only

### Build Status

Branch          | GH Actions | Appveyor | codecov.io | Deps | Docs | Tests |
:-------------: | ---------- | -------- | ---------- | ---- | ---- | ----- |
[`master`](https://github.com/boostorg/unordered/tree/master)   | [![CI](https://github.com/boostorg/unordered/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/boostorg/unordered/actions/workflows/ci.yml)  | [![Build status](https://ci.appveyor.com/api/projects/status/github/boostorg/unordered?branch=master&svg=true)](https://ci.appveyor.com/project/danieljames/unordered-qtwe6/branch/master)   | [![codecov](https://codecov.io/gh/boostorg/unordered/branch/master/graph/badge.svg)](https://codecov.io/gh/boostorg/unordered/branch/master)   | [![Deps](https://img.shields.io/badge/deps-master-brightgreen.svg)](https://pdimov.github.io/boostdep-report/master/unordered.html)   | [![Documentation](https://img.shields.io/badge/docs-master-brightgreen.svg)](https://www.boost.org/doc/libs/master/libs/unordered/doc/html/unordered.html)   | [![Enter the Matrix](https://img.shields.io/badge/matrix-master-brightgreen.svg)](http://www.boost.org/development/tests/master/developer/unordered.html)
[`develop`](https://github.com/boostorg/unordered/tree/develop) | [![CI](https://github.com/boostorg/unordered/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/boostorg/unordered/actions/workflows/ci.yml) | [![Build status](https://ci.appveyor.com/api/projects/status/github/boostorg/unordered?branch=develop&svg=true)](https://ci.appveyor.com/project/danieljames/unordered-qtwe6/branch/develop) | [![codecov](https://codecov.io/gh/boostorg/unordered/branch/develop/graph/badge.svg)](https://codecov.io/gh/boostorg/unordered/branch/develop) | [![Deps](https://img.shields.io/badge/deps-develop-brightgreen.svg)](https://pdimov.github.io/boostdep-report/develop/unordered.html) | [![Documentation](https://img.shields.io/badge/docs-develop-brightgreen.svg)](https://www.boost.org/doc/libs/develop/libs/unordered/doc/html/unordered.html) | [![Enter the Matrix](https://img.shields.io/badge/matrix-develop-brightgreen.svg)](http://www.boost.org/development/tests/develop/developer/unordered.html)

### Directories

| Name        | Purpose                        |
| ----------- | ------------------------------ |
| `doc`       | documentation                  |
| `example`   | examples                       |
| `include`   | headers                        |
| `test`      | unit tests                     |

### More information

* [Ask questions](http://stackoverflow.com/questions/ask?tags=c%2B%2B,boost,boost-unordered)
* [Report bugs](https://github.com/boostorg/unordered/issues): Be sure to mention Boost version, platform and compiler you're using. A small compilable code sample to reproduce the problem is always good as well.
* Submit your patches as pull requests against **develop** branch. Note that by submitting patches you agree to license your modifications under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).
* Discussions about the library are held on the [Boost developers mailing list](http://www.boost.org/community/groups.html#main). Be sure to read the [discussion policy](http://www.boost.org/community/policy.html) before posting and add the `[unordered]` tag at the beginning of the subject line.

