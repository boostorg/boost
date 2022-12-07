+++
title = "Coroutine awaitables"
weight = 20
tags = [ "coroutines", "eager", "lazy", "awaitables" ]
+++

The second part of the support is provided by header `<boost/outcome/coroutine_support.hpp>`
(or `<boost/outcome/experimental/coroutine_support.hpp>` if you want Coroutine support for
Experimental Outcome). This adds into namespace `BOOST_OUTCOME_V2_NAMESPACE::awaitables` (or
`BOOST_OUTCOME_V2_NAMESPACE::experimental::awaitables`) these awaitable types suitable
for returning from a Coroutinised function:

- {{% api "eager<T, Executor = void>" %}}

    An eagerly evaluated Coroutine: invoking `co_await` upon a function returning one
of these immediately begins the execution of the function now. If the function never
suspends, the overhead is similar to calling an ordinary function.

- {{% api "lazy<T, Executor = void>" %}}

    A lazily evaluated Coroutine (often named `task<T>` in most C++ Coroutine
literature): invoking `co_await` upon a function returning one of these causes the
function to be immediately suspended as soon as execution begins. Only resuming
the execution of the coroutine proceeds execution.

- {{% api "generator<T, Executor = void>" %}}

    A lazily evaluated generator of values: the coroutine is resumed to generate
the next value, upon which it is suspended until the next iteration.

- `atomic_eager<T, Executor = void>`

    `eager<T>` does not employ thread synchronisation during resumption of dependent
coroutines which is fine if you do not traverse kernel threads during a
suspend-resume cycle. If you do however potentially traverse kernel threads
during suspend-resume, you ought to use `atomic_eager<T>` instead -- this uses
atomics to synchronise the setting and checking of state to ensure correctness.

- `atomic_lazy<T, Executor = void>`

    Same for `lazy<T>` as `atomic_eager<T>` is for `eager<T>`.
