#include "convex_hull.hpp"
#include "matplotlibcpp.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

namespace plt = matplotlibcpp;

int main()
{
    // Generate random points
    constexpr std::size_t number_of_points = 10000'000;

    std::mt19937 rng;
    std::uniform_real_distribution<double> dist(-10.0, 10.0);
    std::vector<Point<double>> points;
    points.reserve(number_of_points);
    for (std::size_t i = 0; i < number_of_points; ++i)
    {
        points.emplace_back(dist(rng), dist(rng), i);
    }

    ConvexHull convex_hull(points, true);

    auto t1 = std::chrono::high_resolution_clock::now();
    convex_hull.calculateConvexHull();
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Elapsed time: " << (t2 - t1).count() / 1e9 << std::endl;

    const auto &convex_hull_points = convex_hull.getConvexHull();

    std::vector<double> x_points;
    std::vector<double> y_points;
    for (const auto &point : points)
    {
        x_points.emplace_back(point.x);
        y_points.emplace_back(point.y);
    }

    std::vector<double> x_hull_points;
    std::vector<double> y_hull_points;
    for (const auto &point : convex_hull_points)
    {
        x_hull_points.emplace_back(point.x);
        y_hull_points.emplace_back(point.y);
    }

    std::vector<double> x_quadrant1;
    std::vector<double> y_quadrant1;

    std::vector<double> x_quadrant2;
    std::vector<double> y_quadrant2;

    std::vector<double> x_quadrant3;
    std::vector<double> y_quadrant3;

    std::vector<double> x_quadrant4;
    std::vector<double> y_quadrant4;

    const auto &quadrant_points = convex_hull.getQuadrantPoints();
    {
        x_quadrant1.push_back(std::get<0>(quadrant_points).first_point.x);
        x_quadrant1.push_back(std::get<0>(quadrant_points).last_point.x);
        x_quadrant1.push_back(std::get<0>(quadrant_points).root_point.x);

        x_quadrant2.push_back(std::get<1>(quadrant_points).first_point.x);
        x_quadrant2.push_back(std::get<1>(quadrant_points).last_point.x);
        x_quadrant2.push_back(std::get<1>(quadrant_points).root_point.x);

        x_quadrant3.push_back(std::get<2>(quadrant_points).first_point.x);
        x_quadrant3.push_back(std::get<2>(quadrant_points).last_point.x);
        x_quadrant3.push_back(std::get<2>(quadrant_points).root_point.x);

        x_quadrant4.push_back(std::get<3>(quadrant_points).first_point.x);
        x_quadrant4.push_back(std::get<3>(quadrant_points).last_point.x);
        x_quadrant4.push_back(std::get<3>(quadrant_points).root_point.x);

        y_quadrant1.push_back(std::get<0>(quadrant_points).first_point.y);
        y_quadrant1.push_back(std::get<0>(quadrant_points).last_point.y);
        y_quadrant1.push_back(std::get<0>(quadrant_points).root_point.y);

        y_quadrant2.push_back(std::get<1>(quadrant_points).first_point.y);
        y_quadrant2.push_back(std::get<1>(quadrant_points).last_point.y);
        y_quadrant2.push_back(std::get<1>(quadrant_points).root_point.y);

        y_quadrant3.push_back(std::get<2>(quadrant_points).first_point.y);
        y_quadrant3.push_back(std::get<2>(quadrant_points).last_point.y);
        y_quadrant3.push_back(std::get<2>(quadrant_points).root_point.y);

        y_quadrant4.push_back(std::get<3>(quadrant_points).first_point.y);
        y_quadrant4.push_back(std::get<3>(quadrant_points).last_point.y);
        y_quadrant4.push_back(std::get<3>(quadrant_points).root_point.y);
    }

    plt::plot(x_points, y_points, {{"marker", "."}, {"markersize", "3"}, {"linestyle", "None"}});
    plt::plot(x_hull_points, y_hull_points, {{"marker", "x"}, {"markersize", "3"}});

    plt::plot(x_quadrant1, y_quadrant1, {{"marker", "x"}, {"markersize", "8"}, {"linestyle", "None"}});
    plt::plot(x_quadrant2, y_quadrant2, {{"marker", "x"}, {"markersize", "8"}, {"linestyle", "None"}});
    plt::plot(x_quadrant3, y_quadrant3, {{"marker", "x"}, {"markersize", "8"}, {"linestyle", "None"}});
    plt::plot(x_quadrant4, y_quadrant4, {{"marker", "x"}, {"markersize", "8"}, {"linestyle", "None"}});

    plt::show();
    plt::detail::_interpreter::kill();

    return 0;
}