+++
title = "Outcome (proposed std result)"
description = "Advantages and disadvantages of Outcome and its proposed `std::result<T>`"
weight = 40
+++

Outcome (the library described by this documentation) originated in a negative reaction to then originally proposed `std::expected<T, E>`, though what got eventually standardised as `std::expected<T, E>` looks much more like Outcome's `result<T, E>` than the original Expected. [You can read here how those experiences led me to develop Outcome v1]({{% relref "/history" %}}). Outcome comes in both standalone and Boost editions, and its current design was completed in 2018.

Outcome's core is two workhorse types and a macro:

- {{% api "basic_result<T, E, NoValuePolicy>" %}}

- {{% api "basic_outcome<T, EC, EP, NoValuePolicy>" %}}

- {{% api "BOOST_OUTCOME_TRY(var, expr)" %}}

These three core items are then mixed into a veritable cornucopia of [convenience typedefs]({{% relref "/reference/aliases" %}}) and variations to support a wide range of use cases, including [in C++ coroutines]({{% relref "/tutorial/essential/coroutines" %}}), plus there is extensive plumbing and customisation points for deciding how incompatible types ought to interact, or being notified of lifecycle events (e.g. capture a stack backtrace if a `result<T, E>` is constructed with an error).

Outcome perfectly propagates constexpr, triviality and `noexcept`-ness of each individual operation of the types you configure it with. It never touches dynamic memory allocation, and it has been carefully written so the compiler will optimise it out of codegen entirely wherever that is possible. It is capable of 'true moves' for types which declare themselves 'move bitcopying compatible' i.e. destructor calls on moved-from values are elided. 'True moves' can have a game changing performance gain on types with virtual destructors.

Outcome takes a lot of care to have the least possible impact on build times, and it guarantees that a binary built with it will have stable ABI so it is safe to use in _really_ large C++ codebases (standalone edition only). For interoperation with other languages, it guarantees that C code can work with Outcome data types, and it provides a C macro API header file to help with that.

Outcome recognises Expected-like types and will construct from them, which aids interoperability. [A simplified Result type is proposed for standardisation as `std::result<T>`](https://wg21.link/P1028) where the `E` type is hard coded to a proposed `std::error`. This proposed standardisation has been deployed on billions of devices at the time of writing, and [you can use it today via Experimental.Outcome]({{% relref "/experimental" %}}), the reference implementation.


#### Pros:

- Predictable runtime overhead on the happy path.

- Predictable runtime overhead on the sad path.

- Very little codegen bloat added to binaries (though there is a fixed absolute overhead for support libraries if you use Outcome's bundled error types).

- Neither success nor failure is prioritised during use -- types will implicitly construct from both `T` and `E` if it is unambiguous, so no clunky added markup needed to return an `E`.

- Sad path control flow is required to be explicitly specified in every situation. This significantly reduces the number of sad control flow paths in a code base, making it much easier to test all of them. It also means that sad paths get audited and checked during code reviews.

- Macro `TRY` operator feels a bit unnatural to use, but is a god send to saving visual code clutter when all you want to say is 'handle this failure by asking my caller to handle it'. It also works with non-Outcome types, and has its own suite of customisation points for third party extension.

- Works well in all configurations of C++, including C++ exceptions and RTTI globally disabled.

- Works well on all niche architectures, such as HPC, GPUs, DSPs and microcontrollers, and does not dynamically allocate memory.

#### Cons:

- Sad path control flow is required to be explicitly specified in every situation. For code where failure is extremely unlikely, or is not important because it always results in aborting the current operation, the added visual code clutter is unhelpful.

- Results in branchy code, which is slow -- though predictably so -- for embedded controller CPUs.

- Failure to examine an Outcome type generates a compiler diagnostic, but failure to handle both failure and success does not. This can mean failures or successes get accidentally dropped.

- To prevent variant storage having an outsize impact on build times in the same way widespread use of `std::variant` has, Outcome only implements union storage when both `T` and `E` are trivially copyable or move bitcopying. Otherwise struct storage is used, which means Outcome's types are larger than Expected's. This is because implementing exception guarantees during copies and moves of non-trivially-copyable types in union storage involves a lot of work for the compiler on every use of copy and move, so by using struct storage Outcome reduces build time impact from copies and moves significantly.

    Note that one of the major uses of Outcome types is as the return type from a function, in which case copy elision would happen in C++ 14 and is guaranteed from C++ 17 onwards. This means that the larger footprint of struct storage typically has much less impact in optimised code than might be the case if you store these types inside other types.
