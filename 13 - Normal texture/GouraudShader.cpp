#include "GouraudShader.h"

Vec3f GouraudShader::vertex(size_t face, size_t vert) {
   GouraudShaderBase::vertex(model, light, face, vert);
   return Shader::vertex(face,vert);
}

bool GouraudShader::fragment(Vec3f bary, TGAColor &color) {
   ColorShaderBase::fragment(color);
   return GouraudShaderBase::fragment(bary,color);
}