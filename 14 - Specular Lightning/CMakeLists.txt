cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-anonymous-struct -Wno-nested-anon-types")

include_directories(.)

add_executable(TestAllShaders
        TestAllShaders.cpp
        model.cpp
        Camera.cpp
        Viewport.cpp
        Triangle.cpp
        tgaimage.cpp
        Shader.cpp
        NormalTextureShader.cpp
        GouraudTextureShader.cpp
        FlatTextureShader.cpp
        FlatShader.cpp
        GouraudShader.cpp
        NormalShader.cpp
        PhongShader.cpp
        PhongTextureShader.cpp
        PhongNormalShader.cpp
        PhongNormalTextureShader.cpp
        TGAColor.cpp
        )

add_executable(RotatingLight
        model.cpp
        Camera.cpp
        Viewport.cpp
        Triangle.cpp
        tgaimage.cpp
        Shader.cpp
        PhongShader.cpp
        PhongNormalTextureShader.cpp
        rotatingLight.cpp
        TGAColor.cpp)

add_executable(SpecularPower
        model.cpp
        Camera.cpp
        Viewport.cpp
        Triangle.cpp
        tgaimage.cpp
        Shader.cpp
        PhongShader.cpp
        specularPower.cpp
        TGAColor.cpp)