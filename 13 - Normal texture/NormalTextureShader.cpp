#include "NormalTextureShader.h"

Vec3f NormalTextureShader::vertex(size_t face, size_t vert) {
   return Shader::vertex(face, vert);
}

bool NormalTextureShader::fragment(Vec3f bary, TGAColor &color) {
   color = TGAColor(128,128,128,255);
   return true;
}