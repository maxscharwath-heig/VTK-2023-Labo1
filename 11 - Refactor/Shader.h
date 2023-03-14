#ifndef CPPRENDERER_SHADER_H
#define CPPRENDERER_SHADER_H

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

struct Shader {
   Vec3f light;
   Matrix4x4 view;
   Matrix4x4 projection;
   Matrix4x4 viewport;
   Model* model = nullptr;

   virtual ~Shader() = default;
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor& color);
};

#endif //CPPRENDERER_SHADER_H
