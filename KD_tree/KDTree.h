#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <utility>
#include <algorithm>
#include <numeric>
#include <set>
#include <cmath>

#include "KDNode.h"
#include "Point.h"


class KDTree
{
	KDNode* root;
	int k;

	KDNode* insert(std::vector<Point>& points,  int start,  int end,  int depth);
	Point* nearestPointRecursive(KDNode* root_, const Point& target, int depth, Point* best, double& best_dist);

public:
	KDTree(int k);
	KDTree(const std::set<Point>& points,  int k);
	double distance(const Point& a, const Point& b);
	Point* nearestPoint(const Point& point);
};

