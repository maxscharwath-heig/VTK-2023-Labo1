#include "GouraudTextureShader.h"

Vec3f GouraudTextureShader::vertex(size_t face, size_t vert) {
   TexturedColor::vertex(model, face, vert);
   GouraudShaderBase::vertex(model, light, face, vert);
   return Shader::vertex(face, vert);
}

bool GouraudTextureShader::fragment(Vec3f bary, TGAColor &color) {
   TexturedColor::fragment(bary, color);
   return GouraudShaderBase::fragment(bary,color);
}