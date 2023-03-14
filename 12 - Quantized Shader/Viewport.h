#ifndef CPPRENDERER_VIEWPORT_H
#define CPPRENDERER_VIEWPORT_H

#include "geometry.h"

Matrix4x4 make_viewport(int x, int y, size_t w, size_t h, float zoom = 0.4f);

#endif //CPPRENDERER_VIEWPORT_H
