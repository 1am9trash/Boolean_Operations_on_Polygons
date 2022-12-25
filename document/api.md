# C++ & Python API

## C++ API
- Create Object
  ```cpp
  // bool: is_hole, decide to deal with hole problem or not, default is true
  BooleanPolygon bool_polygon(is_hole); 
  ```
- Initial Polygon
  ```cpp
  // polygon order must be clockwise

  // Method 1: from vector
  std::vector<std::vector<double>> p = {{0, 0}, {5, 5}, {10. 0}}; 
  bool_polygon.p1.set_polygon_by_vector(p);

  // Method 2
  // format:
  // 0 0,
  // 5 5,
  // 10 0;
  std::cin >> bool_polygon.p1;
  ```
- Boolean Operation
  ```cpp
  std::vector<Polygon> polygons = bool_polygon.and_polygon();
  std::vector<Polygon> polygons = bool_polygon.or_polygon();
  std::vector<Polygon> polygons = bool_polygon.diff_polygon();
  ```
- Get Result
  ```cpp
  std::vector<Polygon> polygons = bool_polygon.and_polygon();

  // Method 1: to vector
  for (size_t i = 0; i < polygons.size(); i++)
    std::vector<double> vector_polygon = polygons[i].polygon_to_vector();

  // Method 2
  for (size_t i = 0; i < polygons.size(); i++)
      std::cout << polygons[i];
  ```
- Sample Code
  ```cpp
  BooleanPolygon bool_polygon;
  std::cin >> bool_polygon.p1 >> bool_polygon.p2;
  std::vector<Polygon> polygons = bool_polygon.and_polygon();
  // std::vector<Polygon> polygons = bool_polygon.or_polygon();
  // std::vector<Polygon> polygons = bool_polygon.diff_polygon();
  for (size_t i = 0; i < polygons.size(); i++)
      std::cout << polygons[i];
  ```

## Python API
- Create Object
  ```py
  # bool: is_hole, decide to deal with hole problem or not, default is true
  bp = BooleanPolygon.BooleanPolygon(is_hole=True)
  ```
- Initial Polygon
  ```py
  polygon1 = [[0, 0], [5, 5], [10, 0]]
  bp.p1.set_polygon(polygon1)
  ```
- Boolean Operation
  ```py
  and_result = bp.and_polygon()
  or_result = bp.and_polygon()
  diff_result = bp.and_polygon()
  ```