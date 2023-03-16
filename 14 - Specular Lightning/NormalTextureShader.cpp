#include "NormalTextureShader.h"

Vec3f NormalTextureShader::vertex(size_t face, size_t vert) {
   TextureShaderBase::vertex(model, face, vert);
   return Shader::vertex(face,vert);
}

bool NormalTextureShader::fragment(Vec3f bary, TGAColor &color) {
   TextureShaderBase::fragment(bary, color);
   Vec3f ptc = varying_uv[0] * bary.x + varying_uv[1] * bary.y + varying_uv[2] * bary.z;
   Vec2i pc(int(round(ptc.x * float(normals->get_width()))),int(round(ptc.y * float(normals->get_height()))));
   TGAColor normal = normals->get(pc.x, pc.y);
   auto n = Vec3f{ float(normal.raw[2]), float(normal.raw[1]), float(normal.raw[0]) }
            - Vec3f{127.5f,127.5f,127.5f};
   n.normalize();

   float pixelIntensity = std::max<float>(0.f,n * light);

   for (int i = 0; i < 3; ++i)
      color.raw[i] = (unsigned char) (float(color.raw[i]) * pixelIntensity);

   return true;
}