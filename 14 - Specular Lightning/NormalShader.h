#ifndef CPPRENDERER_NORMALSHADER_H
#define CPPRENDERER_NORMALSHADER_H

#include "Shader.h"

struct NormalShader :
        public Shader,
        public MonoColor,
        public TexturedNormals,
        public Culler {
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor& color) const;
};


#endif //CPPRENDERER_NORMALSHADER_H
