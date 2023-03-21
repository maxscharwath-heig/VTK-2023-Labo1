#include "FlatShader.h"

Vec3f FlatShader::vertex(size_t face, size_t vert) {
   FlatNormals::vertex(model,light,face,vert);
   return Shader::vertex(face,vert);
}

bool FlatShader::fragment(Vec3f bary, TGAColor &color) const {
   MonoColor::fragment(color);
   auto n = FlatNormals::fragment(bary,color);
   return Culler::isNotCulled(n);
}