#pragma once

#include "geometric_definitions.hpp"
#include "polygon_area.hpp"

Point findPolygonCentroid(const Polygon &polygon)
{
    double cx = 0.0;
    double cy = 0.0;
    Point result;
    const auto &N = polygon.size();
    for (std::size_t i = 0UL; i < N; ++i)
    {
        std::size_t j = (i + 1) % N;
        double factor = (polygon[i].x * polygon[j].y - polygon[j].x * polygon[i].y);
        cx += (polygon[i].x + polygon[j].x) * factor;
        cy += (polygon[i].y + polygon[j].y) * factor;
    }
    double area = polygonArea(polygon);
    area *= 6.0;
    double factor = 1.0 / area;
    cx *= factor;
    cy *= factor;
    result.x = cx;
    result.y = cy;
    return result;
}