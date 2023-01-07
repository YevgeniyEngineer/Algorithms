#include "quad_tree.hpp"
#include <iostream>
#include <random>

constexpr unsigned int NUM_PTS = 10;

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

    return EXIT_SUCCESS;
}