+++
title = "Future ABI stability guarantees"
weight = 9
+++

At the end of December 2021, [as had been intended and signposted from the beginning of development]({{% relref "/history" %}}), standalone Outcome v2.2.3 locked its ABI such that any code built with this Outcome release shall link, without recompilation, with any code built with any future Outcome release. This means that going forth, you are guaranteed that if your library returns an `outcome::`{{% api "result<T, E = varies, NoValuePolicy = policy::default_policy<T, E, void>>" %}} or `outcome::`{{% api "outcome<T, EC = varies, EP = varies, NoValuePolicy = policy::default_policy<T, EC, EP>>" %}} from a public API, consumers of your library are guaranteed 100% compatibility with unrecompiled library binaries when using any future version of Outcome in consuming code.

This is a critical use case for many large scale production use cases in industry, and to my knowledge, no other Outcome-like alternative implements this guarantee at the time of writing[^1]. Note also that Boost.Outcome comes with no ABI guarantees, as the dependencies within Boost that Boost.Outcome uses do not have a stable ABI guarantee.

To ensure this guarantee going forth, a per commit CI step has been added which tests Outcome against the v2.2.3 ABI using these tools:

- [The ABI compliance checker](https://lvc.github.io/abi-compliance-checker/) (using its `abi-dumper` mode, not its translation unit parsing mode which is too brittle).
- [Sourceware's libabigail tooling](https://sourceware.org/libabigail/manual/libabigail-tools.html).

Both tools are independent of one another, and whilst they test using the same mechanism (DWARF debug info extracted from an unoptimised shared library object), they have different implementations.

#### ABI testing implementation notes

Outcome is a header only library, so to turn Outcome into a shared library suitable as input for these tools, we compile a dummy shared library which exports APIs which use Outcome. The coverage of that dummy shared library of Outcome is therefore what is actually ABI tested, rather than of Outcome itself. The dummy library locks the ABI for:

- `basic_result<trivial type, std::error_code, all policies>` (i.e. union storage layout)
- `basic_outcome<trivial type, std::error_code, trivial type, all policies>` 
- `basic_result<non-trivial type, std::error_code, all policies>` (i.e. struct storage layout)
- `basic_outcome<non-trivial type, std::error_code, std::exception_ptr, all policies>`
- `bad_result_access_with<std::error_code>`
- `bad_outcome_access`
- `atomic_eager<int>`
- `atomic_eager<basic_result<trivial type, std::error_code, all policies>>`
- `atomic_eager<basic_result<non-trivial type, std::error_code, all policies>>`
- `atomic_lazy<int>`
- `atomic_lazy<basic_result<trivial type, std::error_code, all policies>>`
- `atomic_lazy<basic_result<non-trivial type, std::error_code, all policies>>`
- `eager<int>`
- `eager<basic_result<trivial type, std::error_code, all policies>>`
- `eager<basic_result<non-trivial type, std::error_code, all policies>>`
- `lazy<int>`
- `lazy<basic_result<trivial type, std::error_code, all policies>>`
- `lazy<basic_result<non-trivial type, std::error_code, all policies>>`

Obviously anything which any of these types touch in their implementation also gets locked in ABI, in so far as the ABI tool can deduce dependent types. If you examine the source code for the dummy shared library, you will see that we go out of our way to encode explicit lists of dependent types in template parameters, to ensure that the ABI tool definitely discovers everything.

{{% notice note %}}
Outcome.Experimental has no ABI guarantees, as WG21 LEWG is actively modifying its design as it approaches the C++ standard.
{{% /notice %}}

The following targets are tested for ABI stability:

1. GCC 7.5 with libstdc++ configured with the C++ 14 standard and x64 architecture.

2. GCC 9.3 with libstdc++ configured with the C++ 17 standard and x64 architecture.

At the time of writing, no POSIX implementation has guaranteed stability on its C++ 20 standard library ABI, so we do not test that.

There is currently no CI coverage of MSVC ABI stability. The ABI compliance checker can test MSVC binaries for ABI stability, however raising the ABI compliance checker on a Github Actions Windows test runner is quite a lot of work. Donations of sufficient test scripting would be welcome. Note that because the Windows and POSIX implementation is almost the same, ABI stability on POSIX will have strong impact on MSVC ABI stability i.e. MSVC ABI is unlikely to break, albeit without CI testing there are no guarantees.


[^1]: libstdc++ implements a strong ABI stability guarantee and thus any future `std::expected<T, E>` implementation it provides will be ABI stable. However Expected offers only a small subset of the functionality which `outcome::result<T, E>` provides.
