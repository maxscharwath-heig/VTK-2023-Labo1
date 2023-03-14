#include "Shader.h"

Vec3f Shader::vertex(size_t face, size_t vert) {
   Vec3f world = model->vert(size_t(model->face(face)[vert]));
   Vec3f screen = viewport * projection * view * VecH(world);
   return screen;
}

void ColorShaderBase::fragment(TGAColor& color) {
   color = this->color;
}

void TextureShaderBase::vertex(Model* model, size_t face, size_t vert) {
   varying_uv[vert] = model->texture(size_t(model->face_texts(face)[vert]));
}

void TextureShaderBase::fragment(Vec3f bary, TGAColor &color) {
   Vec3f ptc = varying_uv[0] * bary.x + varying_uv[1] * bary.y + varying_uv[2] * bary.z;
   Vec2i pc(
           int(round(ptc.x * float(texture->get_width()))),
           int(round(ptc.y * float(texture->get_height()))));
   color = texture->get(pc.x, pc.y);
}

#define World(vertIdx) model->vert(size_t(model->face(face)[(vertIdx)]))
void FlatShaderBase::vertex(Model* model, Vec3f light, size_t face, size_t vert) {
   if(vert == 0) { // inutile de repeter le calcul pour vert != 0
      Vec3f n = (World(1)- World(0)) ^ (World(2) - World(0));
      n.normalize();
      intensity = n * light;
   }
}

bool FlatShaderBase::fragment([[maybe_unused]] Vec3f bary, TGAColor &color) {
   if(intensity < 0) return false;
   for(int i = 0; i < 3; ++i)
      color.raw[i] = (unsigned char)(float(color.raw[i]) * intensity);
   return true;
}

void GouraudShaderBase::vertex(Model* model, Vec3f light, size_t face, size_t vert) {
   Vec3f n = model->normal(size_t(model->face_normals(face)[vert]));
   varying_intensity.raw[vert] = n * light;
}

bool GouraudShaderBase::fragment(Vec3f bary, TGAColor& color) {
   float pixelIntensity = varying_intensity * bary;
   if (pixelIntensity < 0) return false;

   for (int i = 0; i < 3; ++i)
      color.raw[i] = (unsigned char) (float(color.raw[i]) * pixelIntensity);

   return true;
}