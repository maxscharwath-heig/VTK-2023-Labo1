#ifndef CPPRENDERER_GOURAUDSHADER_H
#define CPPRENDERER_GOURAUDSHADER_H

#include "Shader.h"

struct GouraudShader
        : public Shader,
          public ColorShaderBase,
          public GouraudShaderBase {
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor &color);
};

#endif //CPPRENDERER_GOURAUDSHADER_H
