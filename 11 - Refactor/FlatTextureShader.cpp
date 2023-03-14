#include "FlatTextureShader.h"

Vec3f FlatTextureShader::vertex(size_t face, size_t vert) {
   TextureShaderBase::vertex(model, face, vert);
   FlatShaderBase::vertex(model,light,face,vert);
   return Shader::vertex(face,vert);
}

bool FlatTextureShader::fragment(Vec3f bary, TGAColor &color) {
   TextureShaderBase::fragment(bary, color);
   return FlatShaderBase::fragment(bary,color);
}