#ifndef KDTREE_HPP_
#define KDTREE_HPP_

#define THREADED 1

#include <algorithm>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

static std::uint8_t DEFAULT_RECURSION_DEPTH =
    static_cast<std::uint8_t>(floor(log2(std::thread::hardware_concurrency())));

template <typename T, std::size_t dim> using point_t = std::array<T, dim>;
template <typename T, std::size_t dim> class KDTree
{
    using point_t = std::array<T, dim>;

  public:
    // KDtree(const KDTree &) = delete;
    // KDTree &operator=(const KDTree &) = delete;

    template <typename Iterator> KDTree(Iterator begin, Iterator end) : nodes_(begin, end)
    {
#if THREADED
        root_ = buildTreeParallel(0UL, nodes_.size(), 0UL, 0U);
#else
        root_ = buildTree(0UL, nodes_.size(), 0UL);
#endif
    }

    template <typename Iterable, typename IterableType = typename Iterable::value_type>
    KDTree(Iterable values) : nodes_(values.begin(), values.end())
    {
#if THREADED
        root_ = buildTreeParallel(0UL, nodes_.size(), 0UL, 0U);
#else
        root_ = buildTree(0UL, nodes_.size(), 0UL);
#endif
    }

    ~KDTree()
    {
        root_ = nullptr;
        best_ = nullptr;
    }

    point_t nearest(const point_t &point)
    {
        if (root_ == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }
        best_ = nullptr;
        visited_ = 0UL;
        best_dist_ = 0.0;
        nearest(root_, point, 0UL);
        return best_->point_;
    }

    void printTree()
    {
        printTree("", root_, false);
    }

  private:
    struct Node
    {
        Node(const point_t &point) : point_(point), left_(nullptr), right_(nullptr)
        {
        }
        ~Node()
        {
            left_ = nullptr;
            right_ = nullptr;
        }
        point_t point_;
        Node *left_ = nullptr;
        Node *right_ = nullptr;
    };

    Node *root_ = nullptr;
    Node *best_ = nullptr;

    double best_dist_ = 0.0;
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
                std::cout << std::setprecision(2) << *node_it << " ";
            }
            std::cout << std::setprecision(2) << *(node_it) << ")" << std::endl;

            // enter the next tree level - left and right branch
            printTree(prefix + (is_left ? "│   " : "    "), node->left_, true);
            printTree(prefix + (is_left ? "│   " : "    "), node->right_, false);
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
            buildTree(begin, end, index);
        }
        // parallel
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

            nodes_[middle].right_ = this->buildTree(middle + 1, end, index);
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

    void nearest(Node *root, const point_t &point, std::size_t index)
    {
        if (root == nullptr)
        {
            return;
        }
        ++visited_;

        double dist = distanceSquared(root->point_, point);
        if ((best_ == nullptr) || (dist < best_dist_))
        {
            best_dist_ = dist;
            best_ = root;
        }
        if (best_dist_ == 0.0)
        {
            return;
        }
        double delta = root->point_[index] - point[index];
        index = (index + 1) % dim;
        nearest((delta > 0.0) ? root->left_ : root->right_, point, index);
        if (delta * delta >= best_dist_)
        {
            return;
        }
        nearest((delta > 0.0) ? root->right_ : root->left_, point, index);
    }
};

#endif // KDTREE_HPP_
