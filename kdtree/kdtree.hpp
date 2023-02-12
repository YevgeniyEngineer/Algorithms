#ifndef KDTREE_HPP_
#define KDTREE_HPP_

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <execution>
#include <future>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

const static std::uint8_t DEFAULT_RECURSION_DEPTH =
    static_cast<std::uint8_t>(std::floor(std::log2(std::thread::hardware_concurrency())));

template <typename T, std::size_t dim> using point_t = std::array<T, dim>;
template <typename T, std::size_t dim> class KDTree
{
  protected:
    using point_t = std::array<T, dim>;

  public:
    KDTree &operator=(const KDTree &rhs) = delete;
    KDTree(const KDTree &other) = delete;

    explicit KDTree(const typename std::vector<point_t>::iterator &begin,
                    const typename std::vector<point_t>::iterator &end, bool threaded = true)
        : nodes_(begin, end), root_(nullptr)
    {
        if (threaded)
        {
            root_ = buildTreeParallel(0UL, nodes_.size(), 0UL, 0U);
        }
        else
        {
            root_ = buildTree(0UL, nodes_.size(), 0UL);
        }
    }

    explicit KDTree(const std::vector<point_t> &points, bool threaded = true)
        : nodes_(points.begin(), points.end()), root_(nullptr)
    {
        if (threaded)
        {
            root_ = buildTreeParallel(0UL, nodes_.size(), 0UL, 0U);
        }
        else
        {
            root_ = buildTree(0UL, nodes_.size(), 0UL);
        }
    }

    ~KDTree()
    {
        root_ = nullptr;
        visited_ = 0UL;
    }

    point_t nearest(const point_t &point)
    {
        if (root_ == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }

        Node *best = nullptr;
        double best_dist = std::numeric_limits<double>::max();
        this->nearestSearch(root_, point, 0UL, std::ref(best), std::ref(best_dist));

        point_t closest_neighbour = best->point_;
        best = nullptr;

        return closest_neighbour;
    }

    void nearest(const std::vector<point_t> &points, std::vector<point_t> &neighbours, std::uint8_t thread_num = 4U)
    {
        if (root_ == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }
        const auto &number_of_points = points.size();

        neighbours.clear();
        neighbours.resize(number_of_points);

        std::vector<std::size_t> indices;
        indices.resize(number_of_points);
        std::iota(indices.begin(), indices.end(), 0UL);

        std::for_each(std::execution::par, indices.begin(), indices.end(), [&](const std::size_t &i) -> void {
            Node *best = nullptr;
            double best_dist = std::numeric_limits<double>::max();

            this->nearestSearch(root_, points[i], 0UL, std::ref(best), std::ref(best_dist));

            neighbours[i] = best->point_;
            best = nullptr;
        });
    }

    void findNeighborsWithinRadius(const point_t &point, double search_radius, std::vector<point_t> &neighbors,
                                   std::vector<double> &distances, bool return_sorted = true)
    {
        if (root_ == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }

        neighbors.clear();
        distances.clear();

        recursiveNeighbourWithinRadiusSearch(root_, point, search_radius, 0UL, neighbors, distances);

        if (return_sorted && !neighbors.empty())
        {
            sortNeighborsByRadius(neighbors, distances);
        }
    }

    void findNeighborsWithinRadius(const std::vector<point_t> &points, double search_radius,
                                   std::vector<std::vector<point_t>> &neighbors,
                                   std::vector<std::vector<double>> &distances, bool return_sorted = true)
    {
        if (root_ == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }
        else if (points.empty())
        {
            throw std::runtime_error("No points were provided");
        }

        const auto &number_of_points = points.size();

        neighbors.clear();
        distances.clear();

        neighbors.resize(number_of_points);
        distances.resize(number_of_points);

        std::vector<std::size_t> indices;
        indices.resize(number_of_points);
        std::iota(indices.begin(), indices.end(), 0UL);

        std::for_each(std::execution::par, indices.begin(), indices.end(), [&](const std::size_t &i) -> void {
            recursiveNeighbourWithinRadiusSearch(root_, points[i], search_radius, 0UL, neighbors[i], distances[i]);

            if (return_sorted && !neighbors.empty())
            {
                sortNeighborsByRadius(neighbors[i], distances[i]);
            }
        });
    }

    void printTree()
    {
        this->printTree("", root_, false);
    }

  private:
    struct Node
    {
        explicit Node(const point_t &point) : point_(point), left_(nullptr), right_(nullptr){};
        ~Node()
        {
            left_ = nullptr;
            right_ = nullptr;
        };
        point_t point_;
        Node *left_ = nullptr;
        Node *right_ = nullptr;
    };

    Node *root_ = nullptr;
    std::size_t visited_ = 0UL;
    std::vector<Node> nodes_;

    void printTree(const std::string &prefix, const Node *node, bool is_left)
    {
        if (node != nullptr)
        {
            std::cout << prefix;

            std::cout << (is_left ? "├──" : "└──");

            // print the value of the node
            auto node_it = node->point_.begin();
            std::cout << "(";
            for (; node_it != node->point_.end() - 1; ++node_it)
            {
                std::cout << std::setprecision(2) << std::scientific << *node_it << " ";
            }
            std::cout << std::setprecision(2) << std::scientific << *node_it << ")" << std::endl;

            // enter the next tree level - left and right branch
            this->printTree(prefix + (is_left ? "│   " : "    "), node->left_, true);
            this->printTree(prefix + (is_left ? "│   " : "    "), node->right_, false);
        }
    }

    Node *buildTree(std::size_t begin, std::size_t end, std::size_t index)
    {
        if (end <= begin)
        {
            return nullptr;
        }

        std::size_t middle = begin + (end - begin) / 2;
        auto nodes_it = nodes_.begin();
        std::nth_element(
            nodes_it + begin, nodes_it + middle, nodes_it + end,
            [&index](const Node &pt_1, const Node &pt_2) -> bool { return pt_1.point_[index] < pt_2.point_[index]; });

        index = (index + 1) % dim;
        nodes_[middle].left_ = this->buildTree(begin, middle, index);
        nodes_[middle].right_ = this->buildTree(middle + 1, end, index);

        return &nodes_[middle];
    }

    Node *buildTreeParallel(std::size_t begin, std::size_t end, std::size_t index, std::uint8_t recursion_depth = 0U)
    {
        // sequential
        if (recursion_depth > DEFAULT_RECURSION_DEPTH)
        {
            return buildTree(begin, end, index);
        }
        // parallel
        else
        {
            if (end <= begin)
            {
                return nullptr;
            }

            std::size_t middle = begin + (end - begin) / 2;
            auto nodes_it = nodes_.begin();
            std::nth_element(nodes_it + begin, nodes_it + middle, nodes_it + end,
                             [&index](const Node &pt_1, const Node &pt_2) -> bool {
                                 return pt_1.point_[index] < pt_2.point_[index];
                             });

            index = (index + 1) % dim;

            std::future<Node *> future = std::async(std::launch::async, [&]() {
                return this->buildTreeParallel(begin, middle, index, recursion_depth + 1);
            });

            nodes_[middle].right_ = this->buildTreeParallel(middle + 1, end, index, recursion_depth + 1);
            nodes_[middle].left_ = future.get();

            return &nodes_[middle];
        }
    }

    double distanceSquared(const point_t &pt_1, const point_t &pt_2) const
    {
        double dist = 0.0;
        for (std::size_t i = 0; i < dim; ++i)
        {
            double delta = pt_1[i] - pt_2[i];
            dist += delta * delta;
        }
        return dist;
    }

    void nearestSearch(Node *root, const point_t &point, std::size_t index, Node *&best, double &best_dist)
    {
        if (root == nullptr)
        {
            return;
        }

        double dist = this->distanceSquared(root->point_, point);
        if ((best == nullptr) || (dist < best_dist))
        {
            best_dist = dist;
            best = root;
        }

        if (best_dist == 0.0)
        {
            return;
        }

        double delta = root->point_[index] - point[index];
        index = (index + 1) % dim;
        this->nearestSearch((delta > 0.0) ? root->left_ : root->right_, point, index, best, best_dist);

        if (delta * delta >= best_dist)
        {
            return;
        }

        this->nearestSearch((delta > 0.0) ? root->right_ : root->left_, point, index, best, best_dist);
    }

    void sortNeighborsByRadius(std::vector<point_t> &neighbors, std::vector<double> &distances)
    {
        if (neighbors.empty())
        {
            return;
        }

        std::vector<std::size_t> indices;
        indices.reserve(neighbors.size());
        for (std::size_t i = 0; i < neighbors.size(); ++i)
        {
            indices.emplace_back(i);
        }

        std::sort(indices.begin(), indices.end(), [&distances](const std::size_t &idx_1, const std::size_t &idx_2) {
            return (distances[idx_1] < distances[idx_2]);
        });

        std::vector<point_t> neighbours_temp;
        neighbours_temp.reserve(neighbors.size());

        std::vector<double> distances_temp;
        distances_temp.reserve(distances.size());

        for (const std::size_t &index : indices)
        {
            neighbours_temp.emplace_back(std::move(neighbors[index]));
            distances_temp.emplace_back(std::move(distances[index]));
        }

        neighbors = std::move(neighbours_temp);
        distances = std::move(distances_temp);
    }

    void recursiveNeighbourWithinRadiusSearch(Node *root, const point_t &point, double search_radius, std::size_t index,
                                              std::vector<point_t> &neighbours, std::vector<double> &distances)
    {
        if (root == nullptr)
        {
            return;
        }

        double dist = this->distanceSquared(root->point_, point);

        if (dist <= search_radius * search_radius && dist != 0.0)
        {
            neighbours.emplace_back(root->point_);
            distances.emplace_back(dist);
        }

        bool left_subtree = (point[index] - search_radius < root->point_[index]);
        bool right_subtree = (point[index] + search_radius > root->point_[index]);

        index = (index + 1) % dim;

        // If the distance between the target point and the split plane
        // of the current node is less than the search radius,
        // search both the left and right sub-trees.

        if (left_subtree)
        {
            recursiveNeighbourWithinRadiusSearch(root->left_, point, search_radius, index, neighbours, distances);
        }
        if (right_subtree)
        {
            recursiveNeighbourWithinRadiusSearch(root->right_, point, search_radius, index, neighbours, distances);
        }
    }
};

#endif // KDTREE_HPP_
