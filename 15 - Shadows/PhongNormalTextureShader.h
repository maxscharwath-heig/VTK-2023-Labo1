#ifndef CPPRENDERER_PHONGNORMALTEXTURESHADER_H
#define CPPRENDERER_PHONGNORMALTEXTURESHADER_H
#include "Shader.h"

struct PhongNormalTextureShader
        : public Shader,
          public TexturedColor,
          public TexturedNormals,
          public Culler,
          public PhongNormals {
   float ambientWeight = 0.3f, diffuseWeight = 0.7f, specularWeight = 0.f;
   float specularPower = 10.f;
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor &color) const;
};

#endif //CPPRENDERER_PHONGNORMALTEXTURESHADER_H
