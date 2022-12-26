#include "geometric_definitions.hpp"

// Returns whether a 2D polygon is concave
// or convex.
// Convex == 1
// Concave == -1
// Colinear points == 0 (incomputable)
// Assumption is that polygon is simple,
// does not intersect itself or have holes
int isConvex(const Polygon &polygon)
{
    const auto &N = polygon.size();
    if (N < 3)
    {
        return static_cast<int>(Concavity::INCOMPUTABLE);
    }

    int flag = 0;
    for (std::size_t i = 0UL; i < N; ++i)
    {
        std::size_t j = (i + 1) % N;
        std::size_t k = (i + 2) % N;
        double z = (polygon[j].x - polygon[i].x) * (polygon[k].y - polygon[j].y);
        z -= (polygon[j].y - polygon[i].y) * (polygon[k].x - polygon[j].x);
        if (z < 0.0)
        {
            flag |= 1;
        }
        else if (z > 0.0)
        {
            flag |= 2;
        }
        if (flag == 3)
        {
            return static_cast<int>(Concavity::CONCAVE);
        }
    }
    if (flag != 0)
    {
        return static_cast<int>(Concavity::CONVEX);
    }
    else
    {
        return static_cast<int>(Concavity::INCOMPUTABLE);
    }
}