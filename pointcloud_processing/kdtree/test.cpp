#include "kdtree.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <random>
#include <string>

TEST(KDTreeTest, matchesBruteForce)
{
    constexpr std::size_t NUM_PTS = 10'000UL;
    constexpr std::size_t NUM_TEST_PTS = 500UL;
    constexpr std::size_t NUM_DIM = 3UL;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    std::vector<point_t<double, NUM_DIM>> points(NUM_PTS);
    for (std::size_t i = 0UL; i < NUM_PTS; ++i)
    {
        points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    KDTree<double, NUM_DIM> kdtree(points, true);

    std::vector<point_t<double, NUM_DIM>> test_points(NUM_TEST_PTS);
    for (std::size_t i = 0UL; i < NUM_TEST_PTS; ++i)
    {
        test_points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    for (const auto &test_point : test_points)
    {
        // Find closest point using KD-Tree
        auto closest_point_kdtree = kdtree.nearest(test_point);

        // Find closest point using Brute Force
        double best_distance = std::numeric_limits<double>::max();
        point_t<double, NUM_DIM> closest_point_brute_force;
        for (const auto &point : points)
        {
            double dist = 0.0;
            for (std::size_t dim = 0; dim < NUM_DIM; ++dim)
            {
                double delta = point[dim] - test_point[dim];
                dist += delta * delta;
            }
            if (dist < best_distance)
            {
                best_distance = dist;
                closest_point_brute_force = point;
            }
        }

        // Check assertion
        for (std::size_t dim = 0; dim < NUM_DIM; ++dim)
        {
            ASSERT_DOUBLE_EQ(closest_point_kdtree[dim], closest_point_brute_force[dim]);
        }
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}