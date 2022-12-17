#include "concave_hull.hpp"
#include "convex_hull.hpp"

#include "matplotlibcpp.hpp"

#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <random>
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
    int NUMBER_OF_POINTS = 100;

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

        x_values.push_back(x);
        y_values.push_back(y);
        points.push_back(std::move(point));
        // std::cout << "rnd: " << rnd << " x:" << x << " y:" << y << "\n\n";
    }

    int number_of_hull_points = points.size();

    // ----- Construct Convex Hull ------
    auto t1 = std::chrono::high_resolution_clock::now();
    ConvexHull convex_hull(points);
    auto convex_polygon = convex_hull.getResultAsArray(number_of_hull_points);
    auto t2 = std::chrono::high_resolution_clock::now();
    // ----------------------------------

    std::cout << "Constructed convex hull of size " << number_of_hull_points << " from " << points.size() << " points"
              << std::endl;
    std::cout << "Time elapsed constructing convex hull: " << std::chrono::duration<double>(t2 - t1).count()
              << " seconds" << std::endl;

    std::vector<float> hull_x_values;
    hull_x_values.resize(number_of_hull_points);
    std::vector<float> hull_y_values;
    hull_y_values.resize(number_of_hull_points);
    for (int i = 0; i < convex_polygon.size(); ++i)
    {
        hull_x_values[i] = convex_polygon[i].x;
        hull_y_values[i] = convex_polygon[i].y;
    }

    const std::lock_guard<std::mutex> lock(mutex);

    plt::figure_size(1200, 780);
    plt::plot(x_values, y_values, ".");
    plt::plot(hull_x_values, hull_y_values, "r-");
    plt::show();

    return EXIT_SUCCESS;
}