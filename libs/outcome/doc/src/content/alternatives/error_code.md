+++
title = "std error codes"
description = "Advantages and disadvantages of `std::error_code`"
weight = 20
+++

`std::error_code` came originally from `boost::error_code` which was designed around 2008 as part of implementing Filesystem and Networking. They are a simple trivially copyable type offering improved type safety and functionality over C enumerations. [You can read more about how `std::error_code` works here]({{% relref "/motivation/std_error_code" %}}). They were standardised in the C++ 11 standard, and have been available in Boost since 2008.

#### Pros:

- Predictable runtime overhead on the happy path.

- Predictable runtime overhead on the sad path.

- Unbiased syntax equal for both success and failure requiring explicit code written to handle both.

- Very little codegen bloat added to binaries (though there is a fixed absolute overhead for support libraries).

- Once constructed, passing around `std::error_code` instances optimises well, often being passed in CPU registers.

- Works well in all configurations of C++, including C++ exceptions and RTTI globally disabled.

- Works well on all niche architectures, such as HPC, GPUs, DSPs and microcontrollers.

- Ships with every standard library since C++ 11.

#### Cons:

- Failure to write handling code for failure means failures get silently dropped. This is disturbingly easy to do.

- Results in branchy code, which is slow -- though predictably so -- for embedded controller CPUs.

- Because the `std::error_category` instance used in construction comes from a magic static, the compiler inserts an atomic operation around every `std::error_code` construction (e.g. https://godbolt.org/z/oGaf4qe8a). This can impact optimisation on compilers with poor optimisation of atomics.

- The payload of type `int` is incredibly constraining sometimes, especially on 64-bit platforms. It would have been much better if it were `intptr_t` instead of `int`.

- The payload value of all bits zero has silent hard coded semantics which is incompatible with many C enumerations, which start from value zero. This can cause silent dropping of failures in a very hard to debug way.

- How comparisons between disparate code categories (i.e. mapping) is supposed to work is non-obvious, and even standard library maintainers and many members of WG21 have been confused by it in the past.

(Note that this long list of design caveats is what led to the proposed superseding of `std::error_code` with `std::error`, which you can use today in Outcome.Experimental. See [this page for more information]({{% relref "/experimental" %}})).