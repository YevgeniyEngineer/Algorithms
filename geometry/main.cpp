#include "vector.hpp"

using namespace geom;

int main()
{
    Vector<double, 3> a{1, 2, 3};

    auto b = a.getNormalized();

    std::cout << b << std::endl;

    return EXIT_SUCCESS;
}