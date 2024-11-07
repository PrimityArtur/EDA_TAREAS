#include "kmeans.h"

Kmeans::Kmeans(const std::string& filename, int lim) {
    ifstream file(filename);
    if (!file) throw runtime_error("No se pudo abrir el archivo csv");

    string line;
    getline(file,line);
    for (int lineCount = 0; lineCount < lim && getline(file, line); ++lineCount) {
        istringstream iss(line); string svalue;
        vector<double> coords;
        double value; int i = 0;
        while (getline(iss, svalue, ',')) {
            value = std::stod(svalue);
            coords.push_back(value);
            if (lineCount == 0) bounds.emplace_back(value, value);
            else {
                bounds[i].first = std::min(bounds[i].first, value);
                bounds[i].second = std::max(bounds[i].second, value);
            }
            i++;
        }
        points.push_back(std::move(coords));
    }
}

set<Point> Kmeans::chooseCentroids(int k) {
    set<Point> centroids;
    for (int i = 0; i < k; i++) {
        int randNum = rand() % points.size();
        centroids.emplace(points[randNum].coords);
    }
    return centroids;
}

Point Kmeans::calcMean(const vector<Point>& clusterPoints) {
    vector<double>p (clusterPoints[0].coords.size(), 0.0);
    return accumulate(clusterPoints.begin(), clusterPoints.end(), Point(p)) / clusterPoints.size();
}

double Kmeans::distance(const Point& a, const Point& b)
{
    return std::sqrt(std::inner_product(a.coords.begin(), a.coords.end(),
        b.coords.begin(), 0.0, std::plus<>(),
        [](const double& a, const double& b) { return std::pow(a - b, 2); }));
}

Point Kmeans::closest(Point p, const set<Point>& centroids) {
    double DistMin = std::numeric_limits<double>::max();
    Point result;

    for (const auto& centroid : centroids) {
        double currDist = distance(p, centroid);
        if (currDist < DistMin) {
            DistMin = currDist;
            result = centroid;
        }
    }
    return result;
}

unordered_map<Point, vector<Point>> Kmeans::clustering(const set<Point>& centroids) {
    unordered_map<Point, vector<Point>> clusters;
    for (const auto& point : points) clusters[closest(point, centroids)].push_back(point);
    return clusters;
}

unordered_map<Point, vector<Point>> Kmeans::clustering_kd(const set<Point>& centroids)
{
    unordered_map<Point, vector<Point>> clusters;
    KDTree kdTree(centroids, centroids.begin()->coords.size());
    for (const auto& point : points) clusters[*kdTree.nearestPoint(point)].push_back(point);
    return clusters;
}


set<Point> Kmeans::recalculateCentroids(set<Point>& centroids, unordered_map<Point,vector<Point>>& clusters) {
    clusters = clustering(centroids);
    set<Point> newCentroids;
    for (const auto& cluster : clusters) newCentroids.insert(calcMean(cluster.second));
    return newCentroids;
}

set<Point> Kmeans::recalculateCentroids_kd(set<Point>& centroids, unordered_map<Point,vector<Point>>& clusters) {
    clusters = clustering_kd(centroids);
    set<Point> newCentroids;
    for (const auto& cluster : clusters) newCentroids.insert(calcMean(cluster.second));
    return newCentroids;
}

unordered_map<Point, vector<Point>> Kmeans::kmeans(int k, int iterations, bool print) {
    unordered_map<Point, vector<Point>> clusters;
    set<Point> centroids = chooseCentroids(k);

    if (print) printCentroids(centroids,0);
    for (size_t i = 0; i < iterations; i++) {
        set<Point> newCentroids = recalculateCentroids(centroids,clusters);
        if (print) printCentroids(centroids,i+1);
        if (centroids == newCentroids) break;
        centroids = std::move(newCentroids);
    }
    return clusters;
}

unordered_map<Point, vector<Point>> Kmeans::kmeans_kd(int k, int iterations, bool print) {
    unordered_map<Point, vector<Point>> clusters;
    set<Point> centroids = chooseCentroids(k);

    if (print) printCentroids(centroids,0);
    for (size_t i = 0; i < iterations; i++) {
        set<Point> newCentroids = recalculateCentroids_kd(centroids,clusters);
        if (print) printCentroids(centroids,i+1);
        if (centroids == newCentroids) break;
        centroids = newCentroids;
    }
    return clusters;
}

void Kmeans::printCentroids(set<Point>& centroids, int i) {
    printf("\nCentroides %d:\n[ ", i);
    for (auto& c : centroids) { c.print_point(); printf(" "); }
    printf("]");
}

void Kmeans::ExportToJson(unordered_map<Point, vector<Point>> clusters, const std::string& filename) {
    json j;
    int id = 0;
    for (auto& e : clusters) {
        json jpoints = json::array();
        json jcoords; jcoords = json::array();

        for (auto& dim : e.first.coords) jcoords.push_back(dim);  
        for (auto& p : e.second) {
            json jpoint = json::array();
            for (auto& dim : p.coords) jpoint.push_back(dim);
            jpoints.push_back(jpoint);
        }
        j["clusters"][std::to_string(id)].push_back(jcoords);
        j["clusters"][std::to_string(id)].push_back(jpoints);
        id++;
    }
    std::ofstream file(filename);
    file << j.dump(4); // 4 de identacion
    file.close();
}
