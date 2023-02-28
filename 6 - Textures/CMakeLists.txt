cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 11)

add_executable(Texture
        texture.cpp
        model.cpp
        tgaimage.cpp)

