cmake_minimum_required(VERSION 3.14)
project(face_uvmap_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(include)

pybind11_add_module(face_uvmap_cpp src/face_uvmap_cpp.cpp)

target_link_libraries(face_uvmap_cpp PRIVATE Eigen3::Eigen)