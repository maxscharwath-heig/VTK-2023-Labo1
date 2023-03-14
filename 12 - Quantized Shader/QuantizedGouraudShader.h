#ifndef CPPRENDERER_QUANTIZEDGOURAUDSHADER_H
#define CPPRENDERER_QUANTIZEDGOURAUDSHADER_H

#include "GouraudShader.h"

struct QuantizedGouraudShader
        : public GouraudShader {
   unsigned quants = 5;
   virtual bool fragment(Vec3f bary, TGAColor &color);
};

#endif //CPPRENDERER_QUANTIZEDGOURAUDSHADER_H
