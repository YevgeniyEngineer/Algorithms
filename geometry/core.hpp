#ifndef CORE_HPP_
#define CORE_HPP_

#include <cmath>

constexpr double DOUBLE_EPS = 1e-12;
constexpr double FLOAT_EPS = 1e-6;

static bool isEqual(double x, double y)
{
    return std::fabs(x - y) < DOUBLE_EPS;
}

static bool isEqual(float x, float y)
{
    return std::fabs(x - y) < FLOAT_EPS;
}

#endif // CORE_HPP_