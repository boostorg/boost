#include "x.hpp"
#include "xl.hpp"
#include "../top.hpp"

foo::foo() : x(20) {}

int main()
{
    foo y;
    y.x[1] = 0;
    xl();
    top();
    return 0;
}

