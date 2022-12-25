CC=g++
C_FLAGS=-O3 -Wall -std=c++11
C_SHARED_FLAGS=-shared -fPIC

LIB_NAME=BooleanPolygon

SRC_FOLDER=algorithm/src
INC_FOLDER=algorithm/include
DEMO_FOLDER=sample_code
TEST_FOLDER=test

PY_INCLUDE = $(shell python3 -m pybind11 --includes)
PY_CONFIG = $(shell python3-config --includes)
PY_SUFFIX = $(shell python3-config --extension-suffix)

.PHONY: polygon c_demo py_demo fuzz_test speed_test clean
default: polygon

polygon: $(SRC_FOLDER)/bind_algorithm.cpp $(INC_FOLDER)/polygon.hpp $(SRC_FOLDER)/polygon.cpp
	$(CC) $(C_FLAGS) $(C_SHARED_FLAGS) $(PY_INCLUDE) -I$(INC_FOLDER) $(SRC_FOLDER)/bind_algorithm.cpp $(SRC_FOLDER)/polygon.cpp -o $(LIB_NAME)$(PY_SUFFIX) $(PY_CONFIG)

c_demo: $(INC_FOLDER)/polygon.hpp $(SRC_FOLDER)/polygon.cpp $(DEMO_FOLDER)/c_demo.cpp
	$(CC) $(C_FLAGS) -I$(INC_FOLDER) $(SRC_FOLDER)/polygon.cpp $(DEMO_FOLDER)/c_demo.cpp -o c_demo.out
	@echo "Compiled successfully. Use the following command to run c demo."
	@echo "./c_demo.out < ../testcase/[somefile]"

py_demo: polygon
	cd sample_code && python3 py_demo.py

fuzz_test: $(INC_FOLDER)/polygon.hpp $(SRC_FOLDER)/polygon.cpp $(TEST_FOLDER)/create_polygon.cpp $(TEST_FOLDER)/fuzz_test.cpp
	$(CC) $(C_FLAGS) -I$(INC_FOLDER) $(SRC_FOLDER)/polygon.cpp $(TEST_FOLDER)/fuzz_test.cpp -o fuzz_test.out
	./fuzz_test.out

speed_test:
	$(CC) $(C_FLAGS) -I$(INC_FOLDER) $(SRC_FOLDER)/polygon.cpp $(TEST_FOLDER)/compare.cpp -o compare.out
	./compare.out

clean:
	rm *.out || true
	rm -rf *.so __pycache__ .pytest_cache || true