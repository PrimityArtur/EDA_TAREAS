#ifndef KMEANS_H
#define KMEANS_H

#include <set>
#include <vector>
#include <limits>
#include <numeric>
#include <utility>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "json.hpp"
#include "Point.h"
#include "KDTree.h"

using namespace std;
using json = nlohmann::json;

class Kmeans {
public:
	Kmeans(const std::string& filename, int lim);

	unordered_map<Point, vector<Point>> kmeans(int k, int iterations, bool print=false);
	unordered_map<Point, vector<Point>> kmeans_kd(int k, int iterations, bool print = false);
	void ExportToJson(unordered_map<Point, vector<Point>> clusters, const std::string& filename);
private:
	vector<pair<double, double>> bounds;
	vector<Point> points;
	void printCentroids(set<Point>& centroids, int i);
	double distance(const Point& a, const Point& b);
	set<Point> chooseCentroids(int k);
	Point calcMean(const vector<Point>&v);
	Point closest(Point p, const set<Point>& centroids);
	unordered_map<Point, vector<Point>> clustering(const set<Point>& centroids);
	unordered_map<Point, vector<Point>> clustering_kd(const set<Point>& centroids);
	set<Point> recalculateCentroids(set<Point>& centroids, unordered_map<Point,vector<Point>>& clusters);
	set<Point> recalculateCentroids_kd(set<Point>& centroids, unordered_map<Point,vector<Point>>& clusters);
	
};

#endif
