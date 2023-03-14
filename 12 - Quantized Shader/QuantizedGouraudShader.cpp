#include "QuantizedGouraudShader.h"

bool QuantizedGouraudShader::fragment(Vec3f bary, TGAColor& color) {
   ColorShaderBase::fragment(color);

   float pixelIntensity = varying_intensity * bary;
   if (pixelIntensity < 0) return false;
   pixelIntensity = round(pixelIntensity * quants) / quants;
   for (int i = 0; i < 3; ++i)
      color.raw[i] = (unsigned char) (float(color.raw[i]) * pixelIntensity);

   return true;
}