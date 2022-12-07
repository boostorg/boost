#include <iostream>

// tag::source[]
int main()
{
    char* c = nullptr;
    std::cout << "Hello sanitizers\n " << *c;
}
// end::source[]
