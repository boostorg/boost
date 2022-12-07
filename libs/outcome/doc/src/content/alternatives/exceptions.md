+++
title = "std exception throws"
description = "Advantages and disadvantages of C++ exception throws"
weight = 10
+++

(Note that we assume a table-based EH implementation here, a SJLJ EH implementaton would have even happy and sad path runtime overhead. Table-based EH implementations are almost universal on x64, ARM and AArch64 targets).

C++ exception throws came in the original C++ 98 standard -- at that time, not all the major compilers implemented them yet, and several of those who did did not have efficient implementations, plus in the original days some compiler vendors still patented things like EH implementation techniques to try and maintain a competitive advantage over rival compilers. Unlike other C++ features, enabling C++ exceptions on a code base not written with them in mind is not safe, so this led to the C++ ecosystem becoming bifurcated into exceptions-enabled and exceptions-disabled camps. 

#### Pros:

- Zero runtime overhead on the happy path.

- Success-orientated syntax declutters code of failure control flow paths.

- As a built-in language feature, probably has the least impact on optimisation of any failure handling mechanism here.

- Ships with every standard toolchain (though may not work in some, and cannot be safely enabled for many codebases).

#### Cons:

- Unpredictable runtime overhead on the sad path.

- Unacceptable runtime overhead on the sad path for real time applications.

- Adds considerable bloat to produced binaries, which can be unacceptable for some use cases (embedded).

- Requires RTTI to be enabled or non-standard behaviour results (which is further binary bloat).

- Not available by tradition or convention in several major parts of the C++ ecosystem (embedded, games, audio, to a lesser extent financial).

- Not available in many niche architectures such as HPC, GPUs, DSPs and microcontrollers.

- Most codebases do not invest in adequate correctness testing of the silent proliferation of failure control flow paths which result in C++ exception throwing code (exception throws silently generate multitudes of slight variations of sad path control flows).
