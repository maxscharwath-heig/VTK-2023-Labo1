cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 11)

include_directories(".")

add_executable(Gouraud
        gouraud.cpp
        model.cpp
        tgaimage.cpp)

add_executable(Gouraud_No_Texture
        gouraud_notext.cpp
        model.cpp
        tgaimage.cpp)
