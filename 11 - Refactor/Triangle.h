#ifndef CPPRENDERER_TRIANGLE_H
#define CPPRENDERER_TRIANGLE_H

#include "geometry.h"
#include "tgaimage.h"
#include "Shader.h"

void triangle(Vec3f screen[3], Shader& shader, float* zbuffer, TGAImage &image);

#endif //CPPRENDERER_TRIANGLE_H
