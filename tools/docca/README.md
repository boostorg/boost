# docca

Docca is a set of XSLT transformations which converts Doxygen
XML extracted from Javadoc comments in C++ source code, into
Boost.Quickbook output

These scripts are used by
[Boost.Beast](https://github.com/boostorg/beast/)
to generate the reference.
For example, this
[Javadoc comment](https://github.com/boostorg/beast/blob/b7230f12f16fe7a9f7a1ece5be1f607c8552448a/include/boost/beast/core/buffers_cat.hpp#L68])
is extracted by Doxygen into an intermediate XML representation,
then processed by the XSLT script via Saxon-HE to produce this
[Boost.Quickbook page](https://www.boost.org/doc/libs/1_71_0/libs/beast/doc/html/beast/ref/boost__beast__buffers_cat.html).

## Usage

The following Doxygen configuration settings must be set as indicated:

    DISTRIBUTE_GROUP_DOC = YES

## License

Distributed under the Boost Software License, Version 1.0.
(See accompanying file [LICENSE_1_0.txt](LICENSE_1_0.txt) or copy at
https://www.boost.org/LICENSE_1_0.txt)
