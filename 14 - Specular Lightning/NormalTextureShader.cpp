#include "NormalTextureShader.h"

Vec3f NormalTextureShader::vertex(size_t face, size_t vert) {
   TextureCoordinates::vertex(model,face, vert);
   return Shader::vertex(face,vert);
}

bool NormalTextureShader::fragment(Vec3f bary, TGAColor &color) const {
   TexturedColor::fragment(bary, color);
   auto n = TexturedNormals::fragment(light,bary,color);
   return isNotCulled(n);
}