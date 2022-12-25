#ifndef KDTREE_HPP_
#define KDTREE_HPP_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

// Define a point in 3-Dimensional space
template <typename T> class Point
{
  public:
    explicit Point(const T &x, const T &y, const T &z) : x_(x), y_(y), z_(z){};
    virtual ~Point() = default;

    // Getter methods
    const T &x() const
    {
        return x_;
    }

    const T &y() const
    {
        return y_;
    }

    const T &z() const
    {
        return z_;
    }

  private:
    T x_;
    T y_;
    T z_;
};

// Distance between two points
template <typename T> double distanceSquared(const Point<T> &point_1, const Point<T> &point_2)
{
    auto dx = point_1.x() - point_2.x();
    auto dy = point_1.y() - point_2.y();
    auto dz = point_1.z() - point_2.z();
    return static_cast<double>((dx * dx) + (dy * dy) + (dz * dz));
}

// KDTreeNode
template <typename T> class KDTreeNode
{
  public:
    explicit KDTreeNode<T>(const Point<T> &point) : point_(point), left_(nullptr), right_(nullptr){};
    virtual ~KDTreeNode<T>()
    {
        // std::cout << "Setting KDTreeNode left and right branches to nullptr" << std::endl;

        left_ = nullptr;
        right_ = nullptr;
    };

    // Getter methods
    const KDTreeNode<T> *left() const
    {
        return left_;
    }

    const KDTreeNode<T> *right() const
    {
        return right_;
    }

    const Point<T> &point() const
    {
        return point_;
    }

    // Setter methods
    void left(KDTreeNode<T> *left)
    {
        left_ = left;
    }

    KDTreeNode<T> *left()
    {
        return left_;
    }

    void right(KDTreeNode<T> *right)
    {
        right_ = right;
    }

    KDTreeNode<T> *right()
    {
        return right_;
    }

  private:
    KDTreeNode<T> *left_;
    KDTreeNode<T> *right_;
    Point<T> point_;
};

// KDTree class
template <typename T> class KDTree
{
  public:
    KDTree(const std::vector<Point<T>> &points) : points_(points)
    {
        if (points.empty())
        {
            throw std::runtime_error("At least one point should be provided to KDTree!");
        }

        // Construct tree
        root_ = buildTree(points_, 0);
    };
    ~KDTree() noexcept
    {
        deleteTree(root_);
    };

  private:
    KDTreeNode<T> *root_;
    std::vector<Point<T>> points_;

    // Constructs KDTree from a vector of points
    KDTreeNode<T> *buildTree(std::vector<Point<T>> &points, std::size_t depth)
    {
        if (points.empty())
        {
            return nullptr;
        }

        std::size_t axis = depth % 3;
        // Rearranges the elements in the range [first,last), in such a way that the element at the nth position is the
        // element that would be in that position in a sorted sequence.
        switch (axis)
        {
        case 0:
            std::nth_element(points.begin(), points.end() + points.size() / 2, points.end(),
                             [&](const Point<T> &pt_1, const Point<T> &pt_2) -> bool { return pt_1.x() < pt_2.x(); });
            break;
        case 1:
            std::nth_element(points.begin(), points.end() + points.size() / 2, points.end(),
                             [&](const Point<T> &pt_1, const Point<T> &pt_2) -> bool { return pt_1.y() < pt_2.y(); });
            break;
        case 2:
            std::nth_element(points.begin(), points.end() + points.size() / 2, points.end(),
                             [&](const Point<T> &pt_1, const Point<T> &pt_2) -> bool { return pt_1.z() < pt_2.z(); });
            break;
        }

        KDTreeNode<T> *node = new KDTreeNode<T>(points[points.size() / 2]);
        std::vector<Point<T>> left_points(points.begin(), points.begin() + points.size() / 2);
        std::vector<Point<T>> right_points(points.begin() + points.size() / 2 + 1, points.end());

        node->left(buildTree(left_points, depth + 1));
        node->right(buildTree(right_points, depth + 1));

        return node;
    }

    // Performs a depth-first traversal of the tree and deletes each node. The tree is traversed recursively, first
    // deleting the left subtree, then the right subtree, and finally the node itself. This ensures that all nodes in
    // the tree are properly deleted.
    void deleteTree(KDTreeNode<T> *node)
    {
        if (node == nullptr)
        {
            return;
        }

        deleteTree(node->left());
        // std::cout << "Deleted left node" << std::endl;

        deleteTree(node->right());
        // std::cout << "Deleted right node" << std::endl;

        delete node;
    }
};

#endif // KDTREE_HPP_