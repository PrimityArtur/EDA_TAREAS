#include "KDTree.h"

KDTree::KDTree(int k) : root(NULL), k(k) {}

KDTree::KDTree(const std::set<Point>& points, int k) : k(k)
{
    std::vector<Point> temp(points.begin(), points.end());
	root = insert(temp, 0, temp.size(), 0);
}

KDNode* KDTree::insert(std::vector<Point>& points, int start, int end,  int depth)
{
    if (start >= end) return nullptr;

    int dim = depth % k, median = (start + end) / 2;
    std::nth_element(points.begin() + start, points.begin() + median,
                    points.begin() + end,
                    [dim](const Point& a, const Point& b) {
                        return a.coords[dim] < b.coords[dim];
                    });

    KDNode* node = new KDNode(k);
    node->point = std::move(points[median]);
    node->left = insert(points, start, median, depth + 1);
    node->right = insert(points, median + 1, end, depth + 1);

    return node;
}

Point* KDTree::nearestPoint(const Point& point)
{
    double best_dist = std::numeric_limits<double>::max();
    return nearestPointRecursive(root, point, 0, nullptr, best_dist);
}

Point* KDTree::nearestPointRecursive(KDNode* root_, const Point& target, int depth, Point* best, double& best_dist)
{
    if (!root_) return best;

    double dist = distance(root_->point, target);
    if (dist < best_dist) { best_dist = dist; best = &root_->point; }

    int dim = depth % k;
    bool isLeft = target.coords[dim] < root_->point.coords[dim];
    KDNode* next = isLeft ? root_->left : root_->right;
    KDNode* other = isLeft ? root_->right : root_->left;

    best = nearestPointRecursive(next, target, depth + 1, best, best_dist);
    if (std::abs(target.coords[dim] - root_->point.coords[dim]) < best_dist) {
        best = nearestPointRecursive(other, target, depth + 1, best, best_dist);
    }

    return best;
}

double KDTree::distance(const Point& a, const Point& b)
{
    return std::sqrt(std::inner_product(a.coords.begin(), a.coords.end(),
        b.coords.begin(), 0.0, std::plus<>(),
        [](const double& a, const double& b) { return std::pow(a - b, 2); }));
}