#ifndef CPPRENDERER_NORMALTEXTURESHADER_H
#define CPPRENDERER_NORMALTEXTURESHADER_H

#include "FlatTextureShader.h"

struct NormalTextureShader : public Shader,
        public TexturedColor,
        public TexturedNormals,
        public Culler {
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor& color) const;
};


#endif //CPPRENDERER_NORMALTEXTURESHADER_H
