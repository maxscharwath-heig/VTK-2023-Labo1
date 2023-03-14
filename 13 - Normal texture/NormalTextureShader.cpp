#include "NormalTextureShader.h"

bool NormalTextureShader::fragment(Vec3f bary, TGAColor &color) {
   TextureShaderBase::fragment(bary, color);
   Vec3f ptc = varying_uv[0] * bary.x + varying_uv[1] * bary.y + varying_uv[2] * bary.z;
   Vec2i pc(int(round(ptc.x * float(normals->get_width()))),int(round(ptc.y * float(normals->get_height()))));
   TGAColor normal = normals->get(pc.x, pc.y);
   auto n = Vec3f{ float(normal.raw[2]), float(normal.raw[1]), float(normal.raw[0]) } * (2.f/255.f)
           - Vec3f{1.f,1.f,1.f};
   n.normalize();

   float pixelIntensity = n * light;
   if (pixelIntensity < 0) return false;

   for (int i = 0; i < 3; ++i)
      color.raw[i] = (unsigned char) (float(color.raw[i]) * pixelIntensity);

   return true;
}