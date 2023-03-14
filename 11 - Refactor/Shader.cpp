#include "Shader.h"

Vec3f Shader::vertex(size_t face, size_t vert) {
   Vec3f world = model->vert(size_t(model->face(face)[vert]));
   return viewport * projection * view * VecH(world);
}

bool Shader::fragment(Vec3f bary, TGAColor& color) {
   color = TGAColor(128,128,128,255);
   return true;
}