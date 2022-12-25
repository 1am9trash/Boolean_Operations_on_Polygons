#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <cassert>

#include "polygon.hpp"
#include "create_polygon.cpp"

#define POINT_NUM 1000
#define RANGE 1000
#define TESTCASE 100
#define CHECK_POINT_NUM 10000 


void output_polygon(Polygon &polygon) {
    std::vector<std::vector<double>> vector_polygon = polygon.polygon_to_vector();
    for (size_t j = 0; j < vector_polygon.size(); j++) {
        std::cout << vector_polygon[j][0] << " " << vector_polygon[j][1];
        if (j != vector_polygon.size() - 1)
            std::cout << std::setprecision(15) << ",\n";
        else
            std::cout << std::setprecision(15) << ";\n";
    }
}

void check_and(BooleanPolygon &bool_polygon, std::vector<Polygon> &and_result) {
    for (int i = 0; i < CHECK_POINT_NUM; i++) {
        double x = (double)(rand()) / RAND_MAX * RANGE * 2 - RANGE;
        double y = (double)(rand()) / RAND_MAX * RANGE * 2 - RANGE;

        bool in_p1 = bool_polygon.p1.point_in_polygon(Point(x, y));
        bool in_p2 = bool_polygon.p2.point_in_polygon(Point(x, y));
        bool in_and_result = false;
        for (size_t j = 0; j < and_result.size() && !in_and_result; j++)
            in_and_result |= and_result[j].point_in_polygon(Point(x, y));

        
        assert((in_p1 && in_p2) == in_and_result);
    }
}

void check_or(BooleanPolygon &bool_polygon, std::vector<Polygon> &or_result) {
    for (int i = 0; i < CHECK_POINT_NUM; i++) {
        double x = (double)(rand()) / RAND_MAX * RANGE * 2 - RANGE;
        double y = (double)(rand()) / RAND_MAX * RANGE * 2 - RANGE;

        bool in_p1 = bool_polygon.p1.point_in_polygon(Point(x, y));
        bool in_p2 = bool_polygon.p2.point_in_polygon(Point(x, y));
        bool in_or_result = false;
        for (size_t j = 0; j < or_result.size() && !in_or_result; j++)
            in_or_result |= or_result[j].point_in_polygon(Point(x, y));

        assert((in_p1 || in_p2) == in_or_result);
    }
}

void check_diff(BooleanPolygon &bool_polygon, std::vector<Polygon> &diff_result) {
    for (int i = 0; i < CHECK_POINT_NUM; i++) {
        double x = (double)(rand()) / RAND_MAX * RANGE * 2 - RANGE;
        double y = (double)(rand()) / RAND_MAX * RANGE * 2 - RANGE;

        bool in_p1 = bool_polygon.p1.point_in_polygon(Point(x, y));
        bool in_p2 = bool_polygon.p2.point_in_polygon(Point(x, y));
        bool in_diff_result = false;
        for (size_t j = 0; j < diff_result.size() && !in_diff_result; j++)
            in_diff_result |= diff_result[j].point_in_polygon(Point(x, y));

        assert((in_p1 && !in_p2) == in_diff_result);
    }
}

int main() {
    for (int i = 0; i < TESTCASE; i++) {
        std::cout << "[" << std::setw(4) << i + 1 << "/" << std::setw(4) << TESTCASE << "]: ";

        std::vector<std::vector<double>> p1 = create_polygon(std::vector<double>{0, 0}, RANGE, 0.7, 0.1, POINT_NUM);
        std::vector<std::vector<double>> p2 = create_polygon(std::vector<double>{RANGE / 4, RANGE / 4}, RANGE, 0.7, 0.1, POINT_NUM);

        BooleanPolygon bool_polygon;
        bool_polygon.p1.set_polygon_by_vector(p1);
        bool_polygon.p2.set_polygon_by_vector(p2);

        try {
            std::vector<Polygon> and_result = bool_polygon.and_polygon();
            check_and(bool_polygon, and_result);
            // if (i == TESTCASE - 1) {
            //     output_polygon(bool_polygon.p1);
            //     output_polygon(bool_polygon.p2);
            //     for (size_t j = 0; j < and_result.size(); j++)
            //         output_polygon(and_result[j]);
            // }
        } catch (std::exception &e) {
            std::cerr << "And Exception: " << e.what() << "\n";
        }

        try {
            std::vector<Polygon> or_result = bool_polygon.or_polygon();
            check_or(bool_polygon, or_result);
            // if (i == TESTCASE - 1) {
            //     output_polygon(bool_polygon.p1);
            //     output_polygon(bool_polygon.p2);
            //     for (size_t j = 0; j < or_result.size(); j++)
            //         output_polygon(or_result[j]);
            // }
        } catch (std::exception &e) {
            std::cerr << "Or Exception: " << e.what() << "\n";
        }

        try {
            std::vector<Polygon> diff_result = bool_polygon.diff_polygon();
            check_diff(bool_polygon, diff_result);
            // if (i == TESTCASE - 1) {
            //     output_polygon(bool_polygon.p1);
            //     output_polygon(bool_polygon.p2);
            //     for (size_t j = 0; j < diff_result.size(); j++)
            //         output_polygon(diff_result[j]);
            // }
        } catch (std::exception &e) {
            std::cerr << "And Exception: " << e.what() << "\n";
        }

        std::cout << "intersection cnt: " << bool_polygon.intersection_cnt << "\n";  
    }

    return 0;
}