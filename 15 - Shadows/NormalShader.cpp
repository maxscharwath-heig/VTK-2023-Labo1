#include "NormalShader.h"

Vec3f NormalShader::vertex(size_t face, size_t vert) {
   TextureCoordinates::vertex(model,face, vert);
   return Shader::vertex(face,vert);
}

bool NormalShader::fragment(Vec3f bary, TGAColor &color) const {
   MonoColor::fragment(color);
   auto n = TexturedNormals::fragment(light,bary,color);
   return isNotCulled(n);
}