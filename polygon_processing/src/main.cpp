#include "concave_hull.hpp"
#include "convex_hull.hpp"

#include "matplotlibcpp.hpp"

#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

static constexpr int N = 10000;

std::random_device r;
std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
std::mt19937 eng(seed); // object which produces random bits

std::uniform_int_distribution<> dist(0, N *N - 1); // encapsulates the correct method of turning
                                                   // random bits into random numbers in the range
                                                   // [0, N)

namespace plt = matplotlibcpp;
std::mutex mutex;

int main()
{
    int NUMBER_OF_POINTS = 1000000;

    using namespace polygon_processing;

    std::vector<double> x_values;
    std::vector<double> y_values;
    std::deque<point_t> points;
    x_values.reserve(NUMBER_OF_POINTS);
    y_values.reserve(NUMBER_OF_POINTS);
    for (int i = 0; i < NUMBER_OF_POINTS; i++)
    {
        int rnd = dist(eng);
        double x = rnd % N; // used of % here is just fine
        double y = rnd / N;
        polygon_processing::point_t point;
        point.x = x;
        point.y = y;
        point.index = i;

        x_values.push_back(x);
        y_values.push_back(y);
        points.push_back(std::move(point));
        // std::cout << "rnd: " << rnd << " x:" << x << " y:" << y << "\n\n";
    }

    int number_of_hull_points = points.size();

    // ----- Construct Convex Hull ------
    auto t1 = std::chrono::high_resolution_clock::now();

    ConvexHull convex_hull(points);
    auto convex_points = convex_hull.getResultAsArray(number_of_hull_points);

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Constructed convex hull of size " << number_of_hull_points << " from " << points.size() << " points"
              << std::endl;
    std::cout << "Time elapsed constructing convex hull: " << std::chrono::duration<double>(t2 - t1).count()
              << " seconds" << std::endl;
    // ----------------------------------

    // ----- Construct Concave Hull ------
    t1 = std::chrono::high_resolution_clock::now();

    // Convert points to appropriate format
    std::vector<std::array<float, 2>> points_;
    points_.reserve(points.size());
    for (const auto &point : points)
    {
        points_.push_back({point.x, point.y});
    }

    // Convert convex hull indices to appropriate format
    std::vector<int> convex_hull_indices_;
    convex_hull_indices_.reserve(number_of_hull_points);
    for (const auto &point : convex_points)
    {
        convex_hull_indices_.push_back(point.index);
    }

    // Construct concave hull
    auto concave_points = Concaveman<float, 16>(points_, convex_hull_indices_, 2.0);

    t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Constructed concave hull of size " << concave_points.size() << " from " << points.size() << " points"
              << std::endl;
    std::cout << "Time elapsed constructing concave hull: " << std::chrono::duration<double>(t2 - t1).count()
              << " seconds" << std::endl;
    // ----------------------------------

    const std::lock_guard<std::mutex> lock(mutex);

    std::vector<float> convex_hull_x_values;
    convex_hull_x_values.resize(convex_points.size());
    std::vector<float> convex_hull_y_values;
    convex_hull_y_values.resize(convex_points.size());
    for (int i = 0; i < convex_points.size(); ++i)
    {
        convex_hull_x_values[i] = convex_points[i].x;
        convex_hull_y_values[i] = convex_points[i].y;
    }

    std::vector<float> concave_hull_x_values;
    concave_hull_x_values.resize(concave_points.size());
    std::vector<float> concave_hull_y_values;
    concave_hull_y_values.resize(concave_points.size());
    for (int i = 0; i < concave_points.size(); ++i)
    {
        concave_hull_x_values[i] = concave_points[i][0];
        concave_hull_y_values[i] = concave_points[i][1];
    }

    try
    {
        plt::figure_size(1200, 780);
        plt::plot(x_values, y_values, ".");
        plt::named_plot("convex hull", convex_hull_x_values, convex_hull_y_values, "r-");
        plt::named_plot("concave hull", concave_hull_x_values, concave_hull_y_values, "g-");
        plt::legend();
        plt::title("Polygon simplification");
        plt::show();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}