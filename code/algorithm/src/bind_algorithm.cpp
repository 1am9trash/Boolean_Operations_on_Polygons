#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <vector>

#include "polygon.hpp"


namespace py = pybind11;

PYBIND11_MODULE(BooleanPolygon, m) {
    m.doc() = "boolean operations on polygons";

    py::class_<Polygon>(m, "Polygon")
        .def("set_polygon", &Polygon::set_polygon_by_vector);

    py::class_<BooleanPolygon>(m, "BooleanPolygon")
        .def(py::init<bool, bool, bool>(), 
            pybind11::arg("p1_clockwise")=true, 
            pybind11::arg("p2_clockwise")=false,
            pybind11::arg("is_hole")=true
        )
        .def_readonly("p1", &BooleanPolygon::p1)
        .def_readonly("p2", &BooleanPolygon::p2)
        .def("and_polygon", [](BooleanPolygon &bool_polygon) { 
            std::vector<Polygon> polygon = bool_polygon.and_polygon();
            std::vector<std::vector<std::vector<double>>> vector_polygon;
            for (size_t i = 0; i < polygon.size(); i++)
                vector_polygon.push_back(polygon[i].polygon_to_vector());
            return vector_polygon;
        })
        .def("or_polygon", [](BooleanPolygon &bool_polygon) { 
            std::vector<Polygon> polygon = bool_polygon.or_polygon();
            std::vector<std::vector<std::vector<double>>> vector_polygon;
            for (size_t i = 0; i < polygon.size(); i++)
                vector_polygon.push_back(polygon[i].polygon_to_vector());
            return vector_polygon;
        })
        .def("diff_polygon", [](BooleanPolygon &bool_polygon) { 
            std::vector<Polygon> polygon = bool_polygon.diff_polygon();
            std::vector<std::vector<std::vector<double>>> vector_polygon;
            for (size_t i = 0; i < polygon.size(); i++)
                vector_polygon.push_back(polygon[i].polygon_to_vector());
            return vector_polygon;
        });
}