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

using std::cout;
using std::ostream;
using std::vector;

class Point
{
public:
	std::vector<double> coords;

	Point();
	Point(int k);
	Point(const std::vector<double>& coords_);
	Point(std::vector<double>&& coords_);
	void setPoint(const std::vector<double>& p);
	Point operator+(const Point& q) const;
	Point operator/(double k) const;
	bool operator==(const Point& q) const;
	bool operator<(const Point& q) const;
	void print_point() const;
};

struct orderPointByCoord {
	size_t coord;

	orderPointByCoord(size_t coord) : coord(coord) {}

	bool operator()(const Point& a, const Point& b) const {
		return a.coords[coord] < b.coords[coord];
	}
};

namespace std {
	template <>
	struct hash<Point> {
		size_t operator()(const Point& p) const {
			return std::accumulate(p.coords.begin(), p.coords.end(), size_t(0),
				[](size_t seed, double val) {
					return seed ^ (std::hash<double>()(val));
				});
		}
	};
}