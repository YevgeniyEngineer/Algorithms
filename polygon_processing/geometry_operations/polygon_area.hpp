#include "geometric_definitions.hpp"

double polygonArea(const Polygon &polygon)
{
    double area = 0.0;
    const auto &N = polygon.size();
    for (std::size_t i = 0UL; i < N; ++i)
    {
        std::size_t j = (i + 1) % N;
        area += polygon[i].x * polygon[j].y;
        area -= polygon[i].y * polygon[j].x;
    }
    area /= 2.0;
    return (area < 0.0 ? -area : area);
}