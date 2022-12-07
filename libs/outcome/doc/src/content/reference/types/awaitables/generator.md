+++
title = "`generator<T, Executor = void>`"
description = "A lazily evaluated coroutine generator with Outcome customisation."
+++

This is a classic coroutine generator whereby the coroutine is resumed to calculate
the next value, and is suspended upon yielding that value. If the value being
yielded is an Outcome type, special semantics are used if the coroutine throws
an exception (see below).

The `Executor` template parameter is purely for compatibility with third party software
such as [ASIO](https://think-async.com/Asio/), and this awaitable can be directly used
by ASIO.

Example of use:

```c++
generator<int> func(int x)
{
  while(x >= 0)
  {
    co_yield x--;
  }
}
...
// Creates the coroutine, immediately suspending it.
auto f = func(5);
// If the coroutine has another value to yield ...
while(f)
{
  // Get the next value from the coroutine
  int r = f();
  ...
}
```

`generator<T>` has special semantics if `T` is a type capable of constructing from
an `exception_ptr` or `error_code` -- any exceptions thrown during the function's body
are sent via `T`, preferably via the error code route if {{% api "error_from_exception(" %}}`)`
successfully matches the exception throw. This means that a
{{% api "basic_result<T, E, NoValuePolicy>" %}} or {{% api "basic_outcome<T, EC, EP, NoValuePolicy>" %}} where one of its types is
is compatible will have its `.error()` or `.exception()` set.

Note that `generator<T>` does not otherwise transport exception throws, and rethrows
any exceptions thrown within the coroutine body through the coroutine machinery.
This does not produce reliable consequences in current C++ compilers. You should
therefore wrap the coroutine body in a `try...catch` if `T` is not able to transport
exceptions on its own.

*Requires*: C++ coroutines to be available in your compiler.

*Namespace*: `BOOST_OUTCOME_V2_NAMESPACE::awaitables`

*Header*: `<boost/outcome/coroutine_support.hpp>`
