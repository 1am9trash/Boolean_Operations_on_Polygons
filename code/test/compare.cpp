#include <iostream>
#include <ctime>
#include <deque>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/foreach.hpp>
#include <iomanip>

#include "polygon.hpp"
#include "create_polygon.cpp"

#define POINT_NUM 1000
#define RANGE 100
#define TESTCASE 100

typedef boost::geometry::model::d2::point_xy<double> point;
typedef boost::geometry::model::polygon<point> polygon;


int intersection_cnt = 0;

double check_and(std::vector<std::vector<double>> &vector_p1, std::vector<std::vector<double>> &vector_p2) {
    clock_t start, stop;
    start = clock();
    BooleanPolygon bool_polygon;
    bool_polygon.p1.set_polygon_by_vector(vector_p1);
    bool_polygon.p2.set_polygon_by_vector(vector_p2);
    std::vector<Polygon> or_result = bool_polygon.and_polygon();
    stop = clock();

    intersection_cnt += bool_polygon.intersection_cnt;

    return (double)(stop - start) / CLOCKS_PER_SEC;
}

double check_or(std::vector<std::vector<double>> &vector_p1, std::vector<std::vector<double>> &vector_p2, bool is_hole=true) {
    clock_t start, stop;
    start = clock();
    BooleanPolygon bool_polygon(true, false, is_hole);
    bool_polygon.p1.set_polygon_by_vector(vector_p1);
    bool_polygon.p2.set_polygon_by_vector(vector_p2);
    std::vector<Polygon> or_result = bool_polygon.or_polygon();
    stop = clock();

    return (double)(stop - start) / CLOCKS_PER_SEC;
}

double check_diff(std::vector<std::vector<double>> &vector_p1, std::vector<std::vector<double>> &vector_p2) {
    clock_t start, stop;
    start = clock();
    BooleanPolygon bool_polygon;
    bool_polygon.p1.set_polygon_by_vector(vector_p1);
    bool_polygon.p2.set_polygon_by_vector(vector_p2);
    std::vector<Polygon> or_result = bool_polygon.diff_polygon();
    stop = clock();

    return (double)(stop - start) / CLOCKS_PER_SEC;
}

polygon create_boost_polygon(std::vector<std::vector<double>> &vector_p) {
    polygon p;
    for (size_t i = 0; i < vector_p.size(); i++)
        boost::geometry::append(p.outer(), point(vector_p[i][0], vector_p[i][1]));
    boost::geometry::correct(p);
    return p;
}

double check_boost_and(std::vector<std::vector<double>> &vector_p1, std::vector<std::vector<double>> &vector_p2) {
    clock_t start, stop;
    start = clock();
    polygon p1, p2;
    p1 = create_boost_polygon(vector_p1);
    p2 = create_boost_polygon(vector_p2);
    std::vector<polygon> output;
    boost::geometry::union_(p1, p2, output);
    stop = clock();

    return (double)(stop - start) / CLOCKS_PER_SEC;
}

double check_boost_or(std::vector<std::vector<double>> &vector_p1, std::vector<std::vector<double>> &vector_p2) {
    clock_t start, stop;
    polygon p1, p2;
    start = clock();
    p1 = create_boost_polygon(vector_p1);
    p2 = create_boost_polygon(vector_p2);
    std::vector<polygon> output;
    boost::geometry::intersection(p1, p2, output);
    stop = clock();

    return (double)(stop - start) / CLOCKS_PER_SEC;
}

double check_boost_diff(std::vector<std::vector<double>> &vector_p1, std::vector<std::vector<double>> &vector_p2) {
    clock_t start, stop;
    start = clock();
    polygon p1, p2;
    p1 = create_boost_polygon(vector_p1);
    p2 = create_boost_polygon(vector_p2);
    std::vector<polygon> output;
    boost::geometry::difference(p1, p2, output);
    stop = clock();

    return (double)(stop - start) / CLOCKS_PER_SEC;
}

int main() {
    // sample
    // std::vector<std::vector<double>> vector_p1 = {{0, 0}, {0, 10}, {10, 10}, {10, 0}};
    // std::vector<std::vector<double>> vector_p2 = {{5, 5}, {5, 15}, {15, 15}, {15, 5}};

    std::vector<std::vector<double>> vector_p1 = create_polygon(std::vector<double>{0, 0}, RANGE, 0.7, 0.1, POINT_NUM);
    std::vector<std::vector<double>> vector_p2 = create_polygon(std::vector<double>{RANGE / 2, RANGE / 2}, RANGE, 0.7, 0.1, POINT_NUM);

    double and_time = 0, or_time = 0, or_time_no_hole = 0, diff_time = 0;
    double and_boost_time = 0, or_boost_time = 0, diff_boost_time = 0;
    for (int i = 0; i < TESTCASE; i++) {
        and_time += check_and(vector_p1, vector_p2);
        or_time += check_or(vector_p1, vector_p2);
        or_time_no_hole += check_or(vector_p1, vector_p2, false);
        diff_time += check_diff(vector_p1, vector_p2);
        and_boost_time += check_boost_and(vector_p1, vector_p2);
        or_boost_time += check_boost_or(vector_p1, vector_p2);
        diff_boost_time += check_boost_diff(vector_p1, vector_p2);
    }

    std::cout << "\n";
    std::cout << "testing with " << POINT_NUM << " vertexs.\n";
    std::cout << "average intersection is " << std::setprecision(6) << (double)intersection_cnt / TESTCASE << ".\n";
    std::cout << "\n";
    std::cout << "    library:\n";
    std::cout << "        and: " << std::setprecision(6) << and_time / TESTCASE << "\n";
    std::cout << "         or: " << std::setprecision(6) << or_time / TESTCASE << "\n";
    std::cout << "or(no hole): " << std::setprecision(6) << or_time_no_hole / TESTCASE << "\n";
    std::cout << "       diff: " << std::setprecision(6) << diff_time / TESTCASE << "\n";
    std::cout << "      boost:\n";
    std::cout << "        and: " << std::setprecision(6) << and_boost_time / TESTCASE << "\n";
    std::cout << "         or: " << std::setprecision(6) << or_boost_time / TESTCASE << "\n";
    std::cout << "       diff: " << std::setprecision(6) << diff_boost_time / TESTCASE << "\n";

    return 0;
}