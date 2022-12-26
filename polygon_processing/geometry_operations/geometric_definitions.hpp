#include <vector>

struct Point
{
    Point() : x(0.0), y(0.0){};
    Point(double x1, double y1) : x(x1), y(y1){};

    // Checks equality of this point with another point
    bool operator==(const Point &right) const
    {
        return (x == right.x && y == right.y);
    };

    double x, y;
};

struct Line
{
    Point p1, p2;
};

struct Triangle
{
    Point p1, p2, p3;
};

enum class Concavity
{
    CONCAVE = -1,
    INCOMPUTABLE = 0,
    CONVEX = 1
};

enum class Overlap
{
    INSIDE = 0,
    OUTSIDE = 1
};

using Polygon = std::vector<Point>;