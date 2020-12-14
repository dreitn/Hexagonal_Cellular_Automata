cmake_minimum_required(VERSION 3.17)
project(Hexagonal_Cellular_Automata)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -lOpenCL")

configure_file(kernel.cl kernel.cl COPYONLY)

add_executable(Hexagonal_Cellular_Automata main.cpp omp.cpp serial.cpp cell.cpp base.cpp Timer.cpp)
