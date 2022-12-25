#include "polygon.hpp"


int main() {
    BooleanPolygon bool_polygon;

    std::cin >> bool_polygon.p1 >> bool_polygon.p2;

    std::cout << "Raw Polygon 1:\n" << bool_polygon.p1 << "\n";
    std::cout << "Raw Polygon 2:\n" << bool_polygon.p2 << "\n";

    std::vector<Polygon> polygons = bool_polygon.and_polygon();
    // std::vector<Polygon> polygons = bool_polygon.or_polygon();
    // std::vector<Polygon> polygons = bool_polygon.diff_polygon();

    std::cout << "Processed Polygon 1:\n" << bool_polygon.p1 << "\n";
    std::cout << "Processed Polygon 2:\n" << bool_polygon.p2 << "\n";

    std::cout << "And Result:\n";
    for (size_t i = 0; i < polygons.size(); i++)
        std::cout << polygons[i];

    return 0;
}