#include "NormalTextureShader.h"

Vec3f NormalTextureShader::vertex(size_t face, size_t vert) {
   TexturedColor::vertex(model, face, vert);
   return Shader::vertex(face,vert);
}

bool NormalTextureShader::fragment(Vec3f bary, TGAColor &color) {
   TexturedColor::fragment(bary, color);
   TGAColor normal = TextureCoordinates::getValue(normals, bary);
   auto n = Vec3f{ float(normal.raw[2]), float(normal.raw[1]), float(normal.raw[0]) }
            - Vec3f{127.5f,127.5f,127.5f};
   n.normalize();
   applyShade(color,n*light);
   return isNotCulled(n);
}