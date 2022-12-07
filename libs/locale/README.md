# Boost.CI

Part of the [Boost C++ Libraries](http://github.com/boostorg).

Boost.Locale is a library that provides high quality localization facilities in a C++ way.
It was originally designed a part of [CppCMS](http://cppcms.sourceforge.net/) - a C++ Web Framework project and then contributed to Boost.

Boost.Locale gives powerful tools for development of cross platform localized software - the software that talks to users in their language.

Provided Features:

- Correct case conversion, case folding and normalization.
- Collation (sorting), including support for 4 Unicode collation levels.
- Date, time, timezone and calendar manipulations, formatting and parsing, including transparent support for calendars other than Gregorian.
- Boundary analysis for characters, words, sentences and line-breaks.
- Number formatting, spelling and parsing.
- Monetary formatting and parsing.
- Powerful message formatting (string translation) including support for plural forms, using GNU catalogs.
- Character set conversion.
- Transparent support for 8-bit character sets like Latin1
- Support for `char` and `wchar_t`
- Experimental support for C++0x `char16_t` and `char32_t` strings and streams.

Boost.Locale enhances and unifies the standard library's API the way it becomes useful and convenient for development of cross platform and "cross-culture" software.

In order to achieve this goal Boost.Locale uses the-state-of-the-art Unicode and Localization library: ICU - International Components for Unicode.

Boost.Locale creates the natural glue between the C++ locales framework, iostreams, and the powerful ICU library.

Boost.Locale provides non-ICU based localization support as well.
It is based on the operating system native API or on the standard C++ library support.
Sacrificing some less important features, Boost.Locale becomes less powerful but lighter and easier to deploy and use library.

### License

Distributed under the [Boost Software License, Version 1.0](https://www.boost.org/LICENSE_1_0.txt).

### Properties

* C++03

### Build Status

Branch          | GH Actions | Appveyor | codecov.io | Deps | Docs | Tests |
:-------------: | ---------- | -------- | ---------- | ---- | ---- | ----- |
[`master`](https://github.com/boostorg/locale/tree/master)   | [![CI](https://github.com/boostorg/locale/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/boostorg/locale/actions/workflows/ci.yml)  | [![Build status](https://ci.appveyor.com/api/projects/status/github/boostorg/locale?branch=master&svg=true)](https://ci.appveyor.com/project/Flamefire/locale/branch/master)   | [![codecov](https://codecov.io/gh/boostorg/locale/branch/master/graph/badge.svg)](https://codecov.io/gh/boostorg/locale/branch/master)   | [![Deps](https://img.shields.io/badge/deps-master-brightgreen.svg)](https://pdimov.github.io/boostdep-report/master/locale.html)   | [![Documentation](https://img.shields.io/badge/docs-master-brightgreen.svg)](https://www.boost.org/doc/libs/master/libs/locale/doc/html/index.html)   | [![Enter the Matrix](https://img.shields.io/badge/matrix-master-brightgreen.svg)](http://www.boost.org/development/tests/master/developer/locale.html)
[`develop`](https://github.com/boostorg/locale/tree/develop) | [![CI](https://github.com/boostorg/locale/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/boostorg/locale/actions/workflows/ci.yml) | [![Build status](https://ci.appveyor.com/api/projects/status/github/boostorg/locale?branch=develop&svg=true)](https://ci.appveyor.com/project/Flamefire/locale/branch/develop) | [![codecov](https://codecov.io/gh/boostorg/locale/branch/develop/graph/badge.svg)](https://codecov.io/gh/boostorg/locale/branch/develop) | [![Deps](https://img.shields.io/badge/deps-develop-brightgreen.svg)](https://pdimov.github.io/boostdep-report/develop/locale.html) | [![Documentation](https://img.shields.io/badge/docs-develop-brightgreen.svg)](https://www.boost.org/doc/libs/develop/libs/locale/doc/html/index.html) | [![Enter the Matrix](https://img.shields.io/badge/matrix-develop-brightgreen.svg)](http://www.boost.org/development/tests/develop/developer/locale.html)

### Directories

| Name        | Purpose                        |
| ----------- | ------------------------------ |
| `doc`       | Documentation                  |
| `examples`  | Examples                       |
| `include`   | Headers                        |
| `src`       | Source files                   |
| `test`      | Unit tests                     |

### More information

* [Ask questions](http://stackoverflow.com/questions/ask?tags=c%2B%2B,boost,boost-locale)
* [Report bugs](https://github.com/boostorg/locale/issues): Be sure to mention Boost version, platform and compiler you're using. A small compilable code sample to reproduce the problem is always good as well.
* Submit your patches as pull requests against **develop** branch. Note that by submitting patches you agree to license your modifications under the [Boost Software License, Version 1.0](https://www.boost.org/LICENSE_1_0.txt).
* Discussions about the library are held on the [Boost developers mailing list](http://www.boost.org/community/groups.html#main). Be sure to read the [discussion policy](http://www.boost.org/community/policy.html) before posting and add the `[locale]` tag at the beginning of the subject line.
