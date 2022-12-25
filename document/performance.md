# Performance

## Compare with boost library
- Command
  ```sh
  $ cd code
  $ make speed_test
  ```
- boost library is well-known [library](https://www.boost.org/doc/libs/1_46_1/libs/polygon/doc/index.htm) in c++ language.
- Comparison with boost library on and, or, diff operation (in 1e-6 second)
  - More intersections exist, more efficient my library is.
  - If there are no holes inside the polygons, we may save some times.

  | Library | Vertex Number of Polygon | Number of Intersecion | and | or (without hole) | or | diff |
  | --- | --- | --- | ---: | ---: | ---: | ---: |
  | my lib | 300 | around 2100 | 998 (9.6x) | 1114 (7.5x) | 18794 (0.4x) | 1094 (8.1x) |
  | boost | 300 | around 2100 | 9545 (1.0x) | 8408 (1.0x) | 8408 (1.0x) | 8894 (1.0x) |
  | my lib | 300 | around 50 | 233 (0.7x) | 251 (0.5x) | 576 (0.2x) | 242 (0.6x) |
  | boost | 300 | around 50 | 174 (1.0x) | 131 (1.0x) | 131 (1.0x) | 146 (1.0x) |