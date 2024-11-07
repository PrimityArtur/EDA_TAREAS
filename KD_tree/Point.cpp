#include "Point.h"

Point::Point() {}
Point::Point(int k) : coords(k) {}
Point::Point(const std::vector<double>& coords_) : coords(coords_) {}
Point::Point(std::vector<double>&& coords_) : coords(std::move(coords_)) {}

void Point::setPoint(const std::vector<double>& p) {
    coords = p;
}

bool Point::operator==(const Point& other) const {
    return coords == other.coords;
}

Point Point::operator+(const Point& other) const {
    std::vector<double> result(coords.size());
    std::transform(coords.begin(), coords.end(), other.coords.begin(), result.begin(), std::plus<>());
    return result;
}

Point Point::operator/(double k) const {
    std::vector<double> result(coords.size());
    std::transform(coords.begin(), coords.end(), result.begin(), [k](double c) { return c / k; });
    return result;
}

bool Point::operator<(const Point& other) const {
    return coords < other.coords;
}

void Point::print_point() const {
    printf("(");
    for (size_t i = 0; i < coords.size(); i++) {
        printf("%.7f%s", coords[i], (i < coords.size() - 1) ? ", " : "");
    }
    printf(")");
}