#include "kdtree.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <random>
#include <string>
#include <thread>

TEST(KDTreeTest, matchesBruteForce)
{
    constexpr std::size_t NUM_PTS = 10'000UL;
    constexpr std::size_t NUM_TEST_PTS = 500UL;
    constexpr std::size_t NUM_DIM = 3UL;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    std::vector<point_t<double, NUM_DIM>> points;
    points.reserve(NUM_PTS);
    for (std::size_t i = 0UL; i < NUM_PTS; ++i)
    {
        points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    std::vector<point_t<double, NUM_DIM>> test_points;
    test_points.reserve(NUM_TEST_PTS);
    for (std::size_t i = 0UL; i < NUM_TEST_PTS; ++i)
    {
        test_points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    KDTree<double, NUM_DIM> kdtree(points, true);
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
            // std::cout << closest_point_kdtree[dim] << " " << closest_point_brute_force[dim] << std::endl;
            ASSERT_DOUBLE_EQ(closest_point_kdtree[dim], closest_point_brute_force[dim]);
        }
    }
}

TEST(KDTreeTest, parallelMatchesBruteForce)
{
    constexpr std::size_t NUM_PTS = 10'000UL;
    constexpr std::size_t NUM_TEST_PTS = 500UL;
    constexpr std::size_t NUM_DIM = 3UL;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(-15.0, 15.0);

    std::vector<point_t<double, NUM_DIM>> points;
    points.reserve(NUM_PTS);
    for (std::size_t i = 0UL; i < NUM_PTS; ++i)
    {
        points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    std::vector<point_t<double, NUM_DIM>> test_points;
    test_points.reserve(NUM_TEST_PTS);
    for (std::size_t i = 0UL; i < NUM_TEST_PTS; ++i)
    {
        test_points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    std::vector<point_t<double, NUM_DIM>> closest_points_kdtree;
    closest_points_kdtree.reserve(test_points.size());

    KDTree<double, NUM_DIM> kdtree(points, true);
    kdtree.nearest(test_points, closest_points_kdtree, std::thread::hardware_concurrency());

    for (std::size_t i = 0UL; i < NUM_TEST_PTS; ++i)
    {
        const auto &test_point = test_points[i];
        const auto &closest_point_kdtree = closest_points_kdtree[i];

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
            // std::cout << closest_point_kdtree[dim] << " " << closest_point_brute_force[dim] << std::endl;
            ASSERT_DOUBLE_EQ(closest_point_kdtree[dim], closest_point_brute_force[dim]);
        }
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}