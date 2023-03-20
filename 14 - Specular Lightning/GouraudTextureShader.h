#ifndef CPPRENDERER_GOURAUDTEXTURESHADER_H
#define CPPRENDERER_GOURAUDTEXTURESHADER_H

#include "Shader.h"

struct GouraudTextureShader
        : public Shader,
          public TexturedColor,
          public GouraudNormals {
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor& color) const;
};


#endif //CPPRENDERER_GOURAUDTEXTURESHADER_H
