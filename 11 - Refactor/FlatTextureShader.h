#ifndef CPPRENDERER_FLATTEXTURESHADER_H
#define CPPRENDERER_FLATTEXTURESHADER_H

#include "Shader.h"

struct FlatTextureShader : public Shader {
   TGAImage* texture;
};


#endif //CPPRENDERER_FLATTEXTURESHADER_H
