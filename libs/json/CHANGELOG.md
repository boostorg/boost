Boost 1.80.0

* Add non-const `value::at` overloads.
* Add the ability to manually choose endianness of the platform.
* Add `string::subview()` overload.
* Fix segfault in `array::erase(it)`.
* Fix low performance of `serialize` on libc++.
* Fix ambigious conversion to `std::string_view` on GCC 8.
* Fix parsing on big-endian platforms.
* Fix handling of comment after trailing comma.
* Minor documentation fixes.

Boost 1.79.0

* Standalone mode of the library is removed. Users who wish to
  continue using standalone JSON can switch to
  [the C++ Alliance fork](https://github.com/CPPAlliance/standalone-json.git).
* Add support for JSON Pointer.
* Add `std::error_code` overloads.
* Add `boost::source_location` to `error_codes`.
* Naturally grow string during serialization.

Boost 1.78.0
* Standalone mode of the library is deprecated.
* Allow external libraries to forward declare `value_to` and `value_from`.
* Fixed signed integer overflow in number parsing.
* Documentation fixes.
* Add support for `/Zc:implicitNoexcept-` on MSVC.

Boost 1.77.0:

*  Implicit conversion operator from `string` to `std::string_view`.
* `value_to` supports `TupleLike` types.
* `value_to` and `value_from` support `std::array` and similar types.
* `object` deallocates the correct size.
* Fixed crash when constructing `array` from a pair of iterators that form an
  empty range.
* `key_value_pair` allocates with the correct alignment.
* `std::hash` specializations for json types.

Boost 1.76.0:

* Refactored `value_from` implementation; user customizations are now always
  preferred over library-provided overloads.
* Fixed imprecise parsing for some floating point numbers.
* Fixed link errors in standalone mode, when used alongside Boost.
* Fix Boost.Build builds on GCC 4.8.

--------------------------------------------------------------------------------

Boost 1.75.0:

Initial release.
