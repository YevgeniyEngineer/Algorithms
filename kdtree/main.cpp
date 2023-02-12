#include "kdtree.hpp"

#include <chrono>
#include <cstdio>
#include <random>
#include <thread>

int main()
{
    constexpr std::size_t NUM_PTS = 1'000'000; // 10'000'000; // 100'000;
    constexpr std::size_t NUM_DIM = 3UL;

    std::cout << "Constructing KD-Tree with " << NUM_PTS << " points" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    std::vector<point_t<double, NUM_DIM>> points;
    points.reserve(NUM_PTS);
    for (std::size_t i = 0UL; i < NUM_PTS; ++i)
    {
        double x = dist(gen);
        double y = dist(gen);
        double z = dist(gen);

        // std::printf("(%.3f, %.3f, %.3f)\n", x, y, z);
        points.push_back({x, y, z});
    }

    try
    {
        // Parallel
        {
            // Build the KD-Tree
            auto t1 = std::chrono::high_resolution_clock::now();
            KDTree<double, NUM_DIM> kdtree(points);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for construction of kdtree: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1.0e9 << std::endl;

            // kdtree.printTree();

            point_t<double, NUM_DIM> closest_point;
            double closest_distance;
            point_t<double, NUM_DIM> point_of_interest = {0.5, 0.3, 0.2};

            // Search closest point
            auto t3 = std::chrono::high_resolution_clock::now();
            closest_point = kdtree.nearest(point_of_interest);
            auto t4 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for nearest neighbour search: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count() / 1.0e9 << std::endl;
            std::cout << "Closest point is: (" << closest_point[0] << " " << closest_point[1] << " " << closest_point[2]
                      << ")" << std::endl
                      << std::endl;
        }
        // Sequential
        {
            // Build the KD-Tree
            auto t1 = std::chrono::high_resolution_clock::now();
            KDTree<double, NUM_DIM> kdtree(points, false);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for construction of kdtree: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1.0e9 << std::endl;

            // kdtree.printTree();

            point_t<double, NUM_DIM> closest_point;
            double closest_distance;
            point_t<double, NUM_DIM> point_of_interest = {0.5, 0.3, 0.2};

            // Search closest point
            auto t3 = std::chrono::high_resolution_clock::now();
            closest_point = kdtree.nearest(point_of_interest);
            auto t4 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for nearest neighbour search: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count() / 1.0e9 << std::endl;
            std::cout << "Closest point is: (" << closest_point[0] << " " << closest_point[1] << " " << closest_point[2]
                      << ")" << std::endl
                      << std::endl;
        }
        // Parallel Many Points
        {
            // Build the KD-Tree
            auto t1 = std::chrono::high_resolution_clock::now();
            KDTree<double, NUM_DIM> kdtree(points);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for construction of kdtree: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1.0e9 << std::endl;

            // kdtree.printTree();

            point_t<double, NUM_DIM> closest_point;
            double closest_distance;
            std::vector<point_t<double, NUM_DIM>> points_of_interest;
            for (std::size_t i = 0UL; i < NUM_PTS; ++i)
            {
                points_of_interest.push_back({dist(gen), dist(gen), dist(gen)});
            }

            std::vector<point_t<double, NUM_DIM>> neighbour_points;
            std::size_t number_of_threads = std::thread::hardware_concurrency();

            // Search closest point
            auto t3 = std::chrono::high_resolution_clock::now();
            kdtree.nearest(points_of_interest, neighbour_points, number_of_threads);
            auto t4 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for nearest neighbour search (many-to-many): "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count() / 1.0e9 << std::endl
                      << std::endl;
        }
        // Neighbours within radius
        {
            // Build the KD-Tree
            KDTree<double, NUM_DIM> kdtree(points, true);
            // kdtree.printTree();

            point_t<double, NUM_DIM> point_of_interest{0.5, 0.3, 0.2};

            std::vector<point_t<double, NUM_DIM>> neighbors;
            std::vector<double> distances;

            auto t3 = std::chrono::high_resolution_clock::now();
            kdtree.findNeighborsWithinRadius(point_of_interest, 5.0, neighbors, distances, true);
            auto t4 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for radius neighbour search (one-to-many): "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count() / 1.0e9 << std::endl;

            // Print nearest neighbors within radius
            // for (std::size_t i = 0; i < neighbors.size(); ++i)
            // {
            //     const auto &neighbor = neighbors[i];
            //     const auto &distance = std::sqrt(distances[i]);
            //     std::cout << "(" << point_of_interest[0] << ", " << point_of_interest[1] << ", " <<
            //     point_of_interest[2]
            //               << ") ----> (" << neighbor[0] << ", " << neighbor[1] << ", " << neighbor[2] << ") is within
            //               "
            //               << distance << std::endl;
            // }
        }
        // Neighbors within radius, many to many
        {
            // Recreate points, because this approach requires quite a bit of memory
            constexpr int REDUCED_NUMBER_OF_POINTS = 10000;
            std::vector<point_t<double, NUM_DIM>> reduced_points;
            reduced_points.reserve(REDUCED_NUMBER_OF_POINTS);
            for (std::size_t i = 0UL; i < REDUCED_NUMBER_OF_POINTS; ++i)
            {
                reduced_points.push_back({dist(gen), dist(gen), dist(gen)});
            }

            // Build the KD-Tree
            KDTree<double, NUM_DIM> kdtree(reduced_points, true);

            // Create random search points
            std::vector<point_t<double, NUM_DIM>> points_of_interest;
            for (std::size_t i = 0UL; i < REDUCED_NUMBER_OF_POINTS; ++i)
            {
                points_of_interest.push_back({dist(gen), dist(gen), dist(gen)});
            }

            // Seach for neighbors within radius for each points
            std::vector<std::vector<point_t<double, NUM_DIM>>> neighbors;
            std::vector<std::vector<double>> distances;

            auto t3 = std::chrono::high_resolution_clock::now();
            kdtree.findNeighborsWithinRadius(points_of_interest, 5.0, neighbors, distances, true);
            auto t4 = std::chrono::high_resolution_clock::now();
            std::cout << "Time elapsed for radius neighbour search with " << REDUCED_NUMBER_OF_POINTS
                      << " points (many-to-many): "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count() / 1.0e9 << std::endl;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception!" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
