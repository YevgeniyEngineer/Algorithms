#include "kdtree.hpp"

#include <chrono>
#include <random>

int main()
{
    constexpr std::size_t NUM_PTS = 1'000'000;

    constexpr std::size_t NUM_DIM = 3UL;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    std::vector<point_t<double, NUM_DIM>> points(NUM_PTS);
    for (std::size_t i = 0UL; i < NUM_PTS; ++i)
    {
        points.push_back({dist(gen), dist(gen), dist(gen)});
    }

    try
    {
        // Parallel
        {
            // Build the KD-Tree
            auto t1 = std::chrono::high_resolution_clock::now();
            auto kdtree = KDTree<double, NUM_DIM>(points);
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
                      << ")" << std::endl;
        }
        // Sequential
        {
            // Build the KD-Tree
            auto t1 = std::chrono::high_resolution_clock::now();
            auto kdtree = KDTree<double, NUM_DIM>(points, false);
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
                      << ")" << std::endl;
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
