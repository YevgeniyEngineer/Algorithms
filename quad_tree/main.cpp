#include "quad_tree.hpp"
#include <iostream>
#include <random>

constexpr unsigned int NUM_PTS = 1000;

int main()
{
    // Generate points
    std::vector<Point> random_points;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    for (std::size_t i = 0; i < NUM_PTS; ++i)
    {
        random_points.emplace_back(dist(gen), dist(gen));
    }

    // Print points
    // for (const auto &point : random_points)
    // {
    //     std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    // }

    // Generate Quad Tree
    float x_cent = 0.0f;
    float y_cent = 0.0f;
    float half_width = 10.0f;
    BoundingBox boundary(Point(x_cent, y_cent), half_width);
    QuadTree quad_tree(boundary);
    for (const auto &point : random_points)
    {
        quad_tree.insert(point);
    }

    std::vector<Point> range_points;
    quad_tree.queryRange(BoundingBox(Point(0.0, 0.0), 5.0f), range_points);
    std::cout << "Number of range points: " << range_points.size() << std::endl;
    for (const auto &point : range_points)
    {
        std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    }

    return EXIT_SUCCESS;
}