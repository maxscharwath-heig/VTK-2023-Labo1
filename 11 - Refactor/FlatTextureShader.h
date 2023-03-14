#ifndef CPPRENDERER_FLATTEXTURESHADER_H
#define CPPRENDERER_FLATTEXTURESHADER_H

#include "Shader.h"

struct FlatTextureShader : public Shader,
        public TextureShaderBase,
        public FlatShaderBase {
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor &color);
};


#endif //CPPRENDERER_FLATTEXTURESHADER_H
