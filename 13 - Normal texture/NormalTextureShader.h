#ifndef CPPRENDERER_NORMALTEXTURESHADER_H
#define CPPRENDERER_NORMALTEXTURESHADER_H

#include "GouraudTextureShader.h"

struct NormalTextureShader : public GouraudTextureShader {
   TGAImage* normals;
   virtual bool fragment(Vec3f bary, TGAColor& color);
};


#endif //CPPRENDERER_NORMALTEXTURESHADER_H
