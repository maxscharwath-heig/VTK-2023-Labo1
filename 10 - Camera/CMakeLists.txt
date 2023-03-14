cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(Camera
        camera.cpp
        model.cpp
        tgaimage.cpp)

add_executable(Camera_solution
        camera_solution.cpp
        model.cpp
        tgaimage.cpp)

add_executable(Camera_flat
        camera_flat.cpp
        model.cpp
        tgaimage.cpp)