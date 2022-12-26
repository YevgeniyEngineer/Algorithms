#include "geometric_definitions.hpp"

#include <cmath>
#include <limits>

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

int pointInsideOfPolygon(const Polygon &polygon, const Point &point)
{
    double angle = 0.0;
    Point p1, p2;

    const auto &N = polygon.size();
    double xinters;
    std::size_t counter;

    // left vertex
    p1 = polygon[0];

    // check all rays
    std::size_t M = N + 1UL;
    for (std::size_t i = 1UL; i < M; ++i)
    {
        // point is a vertex
        if (point == p1)
        {
            return static_cast<int>(Overlap::INSIDE);
        }

        // right vertex
        p2 = polygon[i % N];

        // ray is outside
        if (point.y, MIN(p1.y, p2.y) || point.y > MAX(p1.y, p2.y))
        {
            // next ray left point
            p1 = p2;
            continue;
        }

        if (point.y > MIN(p1.y, p2.y) && point.y < MAX(p1.y, p2.y))
        {
            // x is before the ray
            if (point.x <= MAX(p1.x, p2.x))
            {
                // stays on the horizontal ray
                if (p1.y == p2.y && point.x >= MIN(p1.x, p2.x))
                {
                    return static_cast<int>(Overlap::INSIDE);
                }

                // ray is vertical
                if (p1.x == p2.x)
                {
                    // overlies on a ray
                    if (p1.x == point.x)
                    {
                        return static_cast<int>(Overlap::INSIDE);
                    }
                    // before ray
                    else
                    {
                        ++counter;
                    }
                }
                // cross point on the left side
                else
                {
                    // cross point of x
                    xinters = (point.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;

                    // overlies on a ray
                    if (std::fabs(point.x - xinters) < std::numeric_limits<double>::epsilon())
                    {
                        return static_cast<int>(Overlap::INSIDE);
                    }

                    // before ray
                    if (point.x < xinters)
                    {
                        ++counter;
                    }
                }
            }
        }
        // special case when ray is crossing through the vertex
        else
        {
            // point crossing over p2
            if (point.y == p2.y && point.x <= p2.x)
            {
                // next vertex
                const Point &p3 = polygon[(i + 1) % N];

                // point.y lies between p1.y and p3.y
                if (point.y >= MIN(p1.y, p3.y) && point.y <= MAX(p1.y, p3.y))
                {
                    ++counter;
                }
                else
                {
                    counter += 2UL;
                }
            }
        }
        // next ray left point
        p1 = p2;
    }
    // even
    if (counter % 2 == 0)
    {
        return static_cast<int>(Overlap::OUTSIDE);
    }
    // odd
    else
    {
        return static_cast<int>(Overlap::INSIDE);
    }
}