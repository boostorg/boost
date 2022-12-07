+++
title = "LEAF"
description = "Advantages and disadvantages of Lightweight Error Augmentation Framework"
weight = 50
+++

As much as Outcome originated in a negative reaction to the then originally proposed `std::expected<T, E>`, [LEAF](https://boostorg.github.io/leaf/) originated in a negative reaction to Outcome. Some of the perceived issues with Outcome were ([LEAF's own rendition of this can be viewed here](https://boostorg.github.io/leaf/#rationale)):

- Outcome based code is visually cluttered, as both happy and sad paths appear in code.

- Outcome based code generates branchy code at runtime, which impacts low end CPUs and first time code execution.

- Outcome's Result type encodes the type of the error in the function signature, which could be considered as more brittle and problematic for large scale code refactoring[^1].

- Outcome is more strongly opinionated about being the ultimate error handling framework in a program (i.e. all third party custom error handling is assumed to flow into Outcome via customisation point adapters), whereas LEAF is less strongly opinionated, and yet provides equivalent functionality.

LEAF therefore looks more like standard C++ exception handling, but without the non-deterministic sad path at the cost of a slight impact on happy path runtime performance. LEAF's current design was completed in 2020.

If you need an error handling framework which has predictable sad path overhead unlike C++ exceptions, but you otherwise want similar syntax and use experience to C++ exceptions, LEAF is a very solid choice.


#### Pros:

- Very low runtime overhead on the happy path.

- Very low runtime overhead on the sad path.

- Does not cause branchy code to the same extent as Outcome, and the sad path is deterministic unlike with C++ exceptions.

- Very little codegen bloat added to binaries (though there is a fixed absolute overhead for support libraries, most of which can be compiled out using a macro if desired).

- Unlike with any of the preceding options, failures nor successes cannot get unintentionally dropped. This is the same strength of guarantee as with C++ exceptions.

- Works well in most configurations of C++, including C++ exceptions and RTTI globally disabled. Does not dynamically allocate memory.

#### Cons:

- Requires out of band storage for state e.g. thread local storage, or a global synchronised ring buffer[^2].

    - If thread local storage is chosen as the out of band storage, transporting LEAF state across threads requires manual intervention.

    - If a global ring buffer is chosen as the out of band storage, thread synchronisation with global state is required and the ring buffer can wrap which drops state.

- Thread local storage can be problematic or even a showstopper in many niche architectures such as HPC, GPUs, DSPs and microcontrollers. Global synchronised state can introduce an unacceptable performance impact on those architectures.

- Current compilers at the time of writing do not react well to use of thread local storage, it would seem that elision of code generation is inhibited if thread local state gets touched due to pessimistic assumptions about escape analysis. Given that this impacts all of C and C++ due to the same problem with `errno`, it is hoped that future compilers will improve this. Until then, any code which touches thread local storage or magic statics[^3] will not optimise as well as code which does neither.

[^1]: In Outcome, it is strongly recommended that one chooses a single universal error type for all public APIs such as `std::error_code` or `error` from Experimental.Outcome, so if the programmer is disciplined then the function signature does not expose internal error types. Such single universal error types type erase the original error object, but still allow the original error object to be inspected. This avoids 'exception specifications' which are widely known to not scale well.

[^2]: A global synchronised ring buffer implementation does not ship with LEAF, however LEAF exposes customisation points for a bespoke thread local storage implementation which makes implementing one very straightforward.

[^3]: `std::error_code` construction touches a magic static or calls an extern function, and therefore Outcome when combined with `std::error_code` also sees a codegen pessimisation. Experimental Outcome's `error` fixes this historical oversight.
