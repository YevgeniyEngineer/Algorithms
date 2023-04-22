#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

struct Point3D
{
    double x, y, z;
};

struct Face
{
    int a, b, c;
    bool valid;
};

static inline double squared_distance(const Point3D &A, const Point3D &B) noexcept
{
    const auto dx = (A.x - B.x);
    const auto dy = (A.y - B.y);
    const auto dz = (A.z - B.z);
    return dx * dx + dy * dy + dz * dz;
}

static inline Point3D cross_product(const Point3D &A, const Point3D &B) noexcept
{
    return {A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x};
}

static inline double dot_product(const Point3D &A, const Point3D &B) noexcept
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

static inline Point3D subtract(const Point3D &A, const Point3D &B) noexcept
{
    return {A.x - B.x, A.y - B.y, A.z - B.z};
}

static double signed_volume(const Point3D &A, const Point3D &B, const Point3D &C, const Point3D &D) noexcept
{
    return dot_product(cross_product(subtract(B, A), subtract(C, A)), subtract(D, A));
}

static bool above(const Face &F, const Point3D &P, const std::vector<Point3D> &points) noexcept
{
    return signed_volume(points[F.a], points[F.b], points[F.c], P) > 0;
}

void quickhull_recursive(const std::vector<Point3D> &points, const std::vector<int> &indices, const Face &F,
                         std::vector<Face> &hull)
{
    if (indices.empty())
    {
        hull.push_back({F.a, F.b, F.c, true});
        return;
    }

    int farthest = indices[0];
    double max_volume = std::abs(signed_volume(points[F.a], points[F.b], points[F.c], points[farthest]));

    for (const int i : indices)
    {
        const double volume = std::abs(signed_volume(points[F.a], points[F.b], points[F.c], points[i]));
        if (volume > max_volume)
        {
            farthest = i;
            max_volume = volume;
        }
    }

    std::vector<int> indices_a, indices_b, indices_c;
    std::vector<int> new_indices = indices;
    new_indices.erase(std::remove(new_indices.begin(), new_indices.end(), farthest), new_indices.end());

    for (const int i : new_indices)
    {
        if (above({F.a, F.b, farthest, true}, points[i], points))
        {
            indices_a.push_back(i);
        }
        if (above({F.b, F.c, farthest, true}, points[i], points))
        {
            indices_b.push_back(i);
        }
        if (above({F.c, F.a, farthest, true}, points[i], points))
        {
            indices_c.push_back(i);
        }
    }

    quickhull_recursive(points, indices_a, {F.a, F.b, farthest, true}, hull);
    quickhull_recursive(points, indices_b, {F.b, F.c, farthest, true}, hull);
    quickhull_recursive(points, indices_c, {F.c, F.a, farthest, true}, hull);
}

std::vector<Face> quickhull_3D(const std::vector<Point3D> &points)
{
    std::vector<Face> hull;
    if (points.size() < 4)
    {
        std::cerr << "Error: At least 4 non-coplanar points are required." << std::endl;
        return hull;
    }

    // Find the tetrahedron formed by the first 4 non-coplanar points
    int a = 0, b = 1, c = 2, d = 3;
    while (std::abs(signed_volume(points[a], points[b], points[c], points[d])) < 1e-9)
    {
        ++d;
        if (d == points.size())
        {
            std::cerr << "Error: All points are coplanar." << std::endl;
            return hull;
        }
    }

    std::vector<int> indices;
    for (int i = 0; i < points.size(); ++i)
    {
        if ((i != a) && (i != b) && (i != c) && (i != d))
        {
            indices.push_back(i);
        }
    }

    std::vector<int> indices_a, indices_b, indices_c, indices_d;
    for (const int i : indices)
    {
        const auto &point = points[i];
        if (above({a, b, c, true}, point, points))
        {
            indices_a.push_back(i);
        }
        if (above({b, c, d, true}, point, points))
        {
            indices_b.push_back(i);
        }
        if (above({c, d, a, true}, point, points))
        {
            indices_c.push_back(i);
        }
        if (above({d, a, b, true}, point, points))
        {
            indices_d.push_back(i);
        }
    }

    quickhull_recursive(points, indices_a, {a, b, c, true}, hull);
    quickhull_recursive(points, indices_b, {b, c, d, true}, hull);
    quickhull_recursive(points, indices_c, {c, d, a, true}, hull);
    quickhull_recursive(points, indices_d, {d, a, b, true}, hull);

    return hull;
}

int main()
{
    // std::vector<Point3D> points = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
    //                                {1, 1, 1}, {1, 1, 0}, {0, 1, 1}, {1, 0, 1}};

    std::mt19937 rng;
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    std::vector<Point3D> points;
    constexpr int NUMBER_OF_POINTS = 1000;
    for (int i = 0; i < NUMBER_OF_POINTS; ++i)
    {
        points.push_back({dist(rng), dist(rng), dist(rng)});
    }

    auto t1 = std::chrono::steady_clock::now();
    std::vector<Face> hull = quickhull_3D(points);
    auto t2 = std::chrono::steady_clock::now();

    std::cout << "Convex Hull Faces:" << std::endl;
    for (const Face &F : hull)
    {
        if (F.valid)
        {
            std::cout << "(" << points[F.a].x << ", " << points[F.a].y << ", " << points[F.a].z << ") ";
            std::cout << "(" << points[F.b].x << ", " << points[F.b].y << ", " << points[F.b].z << ") ";
            std::cout << "(" << points[F.c].x << ", " << points[F.c].y << ", " << points[F.c].z << ")" << std::endl;
        }
    }

    std::cout << std::endl << "Elapsed time (s): " << (t2 - t1).count() / 1e9 << std::endl;
    std::cout << "Number of faces: " << hull.size() << std::endl;

    return 0;
}
