#include "Shader.h"

void applyShade(TGAColor& color, float intensity) {
   if (intensity < 0.f)
      intensity = 0.f;
   for (int i = 0; i < 3; ++i)
      color.raw[i] = (unsigned char) (float(color.raw[i]) * intensity);
}

Vec3f Shader::vertex(size_t face, size_t vert) {
   Vec3f world = model->vert(size_t(model->face(face)[vert]));
   Vec3f screen = viewport * projection * view * VecH(world);
   return screen;
}

bool Culler::isNotCulled(Vec3f normal) const {
   if(normal*camera >= 0)
      return !frontfaceCulling;
   else
      return !backfaceCulling;
}

void MonoColor::fragment(TGAColor& color) const {
   color = this->color;
}

void TextureCoordinates::vertex(Model* model, size_t face, size_t vert) {
   varying_uv[vert] = model->texture(size_t(model->face_texts(face)[vert]));
}

TGAColor TextureCoordinates::getValue(TGAImage* im, Vec3f bary) const {
   Vec3f pt = varying_uv[0] * bary.x + varying_uv[1] * bary.y + varying_uv[2] * bary.z;
   return im->get(int(round(pt.x * float(im->get_width()))),
                  int(round(pt.y * float(im->get_height()))));
}

void TexturedColor::fragment(Vec3f bary, TGAColor &color) const {
   color = TextureCoordinates::getValue(textureImage, bary);
}

#define World(vertIdx) model->vert(size_t(model->face(face)[(vertIdx)]))
void FlatNormals::vertex(Model* model, Vec3f light, size_t face, size_t vert) {
   if(vert == 0) {
      faceNormal = (World(1)- World(0)) ^ (World(2) - World(0));
      faceNormal.normalize();
      intensity = faceNormal * light;
   }
}

Vec3f FlatNormals::fragment([[maybe_unused]] Vec3f bary, TGAColor &color) const {
   applyShade(color, intensity);
   return faceNormal;
}

void GouraudNormals::vertex(Model* model, Vec3f light, size_t face, size_t vert) {
   Vec3f n = model->normal(size_t(model->face_normals(face)[vert]));
   varying_intensity.raw[vert] = n * light;
}

void GouraudNormals::fragment(Vec3f bary, TGAColor& color) const {
   applyShade(color, varying_intensity * bary);
}

void PhongNormals::vertex(Model* model, size_t face, size_t vert) {
   normals[vert] = model->normal(size_t(model->face_normals(face)[vert]));
}

Vec3f PhongNormals::fragment(Vec3f light, Vec3f bary, TGAColor& color) const {
   Vec3f n = normals[0] * bary.x + normals[1] * bary.y + normals[2] * bary.z;
   applyShade(color,n * light);
   return n;
}

void PhongNormals::specular(Vec3f light, Vec3f camera, Vec3f normal, float power, TGAColor& color) const {
   Vec3f reflexion = normal * 2.f * (normal * light) - light;
   reflexion.normalize();
   float intensity = std::max<float>(0.f,camera * reflexion);
   intensity = pow(intensity,power);
   applyShade(color,intensity);
}

Vec3f TexturedNormals::fragment(Vec3f light, Vec3f bary, TGAColor& color) const {
   TGAColor normal = TextureCoordinates::getValue(normalsImage, bary);
   auto n = Vec3f{ float(normal.raw[2]), float(normal.raw[1]), float(normal.raw[0]) }
            - Vec3f{127.5f,127.5f,127.5f};
   n.normalize();
   applyShade(color, n * light);
   return n;
}
