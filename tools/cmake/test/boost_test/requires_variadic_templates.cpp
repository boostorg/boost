// Copyright 2019 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

template<class... T> int f()
{
    return sizeof...(T);
}

int main()
{
    return f<int, void, float>() == 3? 0: 1;
}
