cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-anonymous-struct -Wno-nested-anon-types")

include_directories(.)

add_executable(NormalTexture
        main.cpp
        model.cpp
        Camera.cpp
        Viewport.cpp
        Triangle.cpp
        tgaimage.cpp
        Shader.cpp
        NormalTextureShader.cpp
        GouraudTextureShader.cpp
        )
