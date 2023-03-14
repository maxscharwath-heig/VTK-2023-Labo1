#ifndef CPPRENDERER_FLATSHADER_H
#define CPPRENDERER_FLATSHADER_H

#include "Shader.h"

struct FlatShader : public Shader,
        public ColorShaderBase,
        public FlatShaderBase {
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor &color);
};

#endif //CPPRENDERER_FLATSHADER_H
