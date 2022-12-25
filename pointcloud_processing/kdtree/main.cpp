#include "kdtree.hpp"

int main()
{
    // Create a list of points
    std::vector<Point<int>> points = {Point(2, 3, 1), Point(5, 4, 5), Point(9, 6, 3),
                                      Point(4, 4, 7), Point(8, 2, 1), Point(7, 5, 2)};

    // Build the KD-Tree
    KDTree kdtree(points);

    // // Search for the nearest neighbor of a given point
    // Point target = Point({3, 4, 1});
    // Point nearest = nearestNeighbor(root, target);
    // std::cout << "Nearest neighbor: (";
    // for (int i = 0; i < nearest.coordinates.size() - 1; i++)
    // {
    //     std::cout << nearest.coordinates[i] << " ";
    // }
    // std::cout << *(nearest.coordinates.end() - 1) << ")" << std::endl;

    // std::cout << "Distances: " << std::endl;
    // for (int i = 0; i < points.size(); ++i)
    // {
    //     auto point = points[i];
    //     auto dx = point.coordinates[0] - target.coordinates[0];
    //     auto dy = point.coordinates[1] - target.coordinates[1];
    //     auto dz = point.coordinates[2] - target.coordinates[2];
    //     auto dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    //     std::cout << "(" << point.coordinates[0] << " " << point.coordinates[1] << " " << point.coordinates[2] << ")"
    //               << " ----> "
    //               << "(" << target.coordinates[0] << " " << target.coordinates[1] << " " << target.coordinates[2] <<
    //               ")"
    //               << " = " << dist << std::endl;
    // }

    return 0;
}