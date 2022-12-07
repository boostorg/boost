# LEAF

> A lightweight error handling library for C++11.

## Documentation

https://boostorg.github.io/leaf/

## Features

* Portable single-header format, no dependencies.
* Tiny code size when configured for embedded development.
* No dynamic memory allocations, even with very large payloads.
* Deterministic unbiased efficiency on the "happy" path and the "sad" path.
* Error objects are handled in constant time, independent of call stack depth.
* Can be used with or without exception handling.

## Support

* [cpplang on Slack](https://Cpplang.slack.com) (use the `#boost` channel)
* [Boost Users Mailing List](https://lists.boost.org/mailman/listinfo.cgi/boost-users)
* [Boost Developers Mailing List](https://lists.boost.org/mailman/listinfo.cgi/boost)

## Distribution

Besides GitHub, there are two other distribution channels:

* LEAF is included in official [Boost](https://www.boost.org/) releases, starting with Boost 1.75.
* For maximum portability, the library is also available in single-header format: simply download [leaf.hpp](https://boostorg.github.io/leaf/leaf.hpp) (direct download link).

Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc. Distributed under the http://www.boost.org/LICENSE_1_0.txt[Boost Software License, Version 1.0].
