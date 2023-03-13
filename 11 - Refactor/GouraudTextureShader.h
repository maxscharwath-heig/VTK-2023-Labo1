#ifndef CPPRENDERER_GOURAUDTEXTURESHADER_H
#define CPPRENDERER_GOURAUDTEXTURESHADER_H

#include "Shader.h"

struct GouraudTextureShader : public Shader {
   TGAImage* texture;
};


#endif //CPPRENDERER_GOURAUDTEXTURESHADER_H
