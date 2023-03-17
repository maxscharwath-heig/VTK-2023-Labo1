#include "NormalTextureShader.h"

Vec3f NormalTextureShader::vertex(size_t face, size_t vert) {
   TexturedColor::vertex(model, face, vert);
   return Shader::vertex(face,vert);
}

bool NormalTextureShader::fragment(Vec3f bary, TGAColor &color) {
   TexturedColor::fragment(bary, color);
   TexturedNormals::fragment(light,bary,color);
   return isNotCulled(TexturedNormals::getNormal(bary));
}