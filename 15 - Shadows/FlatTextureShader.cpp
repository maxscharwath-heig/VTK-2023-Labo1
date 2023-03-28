#include "FlatTextureShader.h"

Vec3f FlatTextureShader::vertex(size_t face, size_t vert) {
   TexturedColor::vertex(model, face, vert);
   FlatNormals::vertex(model, light, face, vert);
   return Shader::vertex(face,vert);
}

bool FlatTextureShader::fragment(Vec3f bary, TGAColor &color) const {
   TexturedColor::fragment(bary, color);
   auto n = FlatNormals::fragment(bary, color);
   return Culler::isNotCulled(n);
}