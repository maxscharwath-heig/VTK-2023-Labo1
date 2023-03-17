#include "GouraudShader.h"

Vec3f GouraudShader::vertex(size_t face, size_t vert) {
   GouraudNormals::vertex(model, light, face, vert);
   return Shader::vertex(face,vert);
}

bool GouraudShader::fragment(Vec3f bary, TGAColor &color) {
   MonoColor::fragment(color);
   return GouraudNormals::fragment(bary, color);
}