#include "foo.h"

foo::foo() : x(20) {}

int main()
{
    foo y;
    y.x[1] = 0;
    return 0;
}

