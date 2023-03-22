#include "TGAColor.h"
#include <algorithm>

TGAColor operator*(TGAColor c, float f) {
   for(size_t i = 0; i < 3; ++i)
      c.raw[i] = static_cast<unsigned char>(
              std::min<float>(255.f,c.raw[i]*f));
   return c;
}

TGAColor operator*(float f, TGAColor const& c) {
   return c*f;
}

TGAColor operator+(TGAColor a, TGAColor const& b) {
   for(size_t i = 0; i < 3; ++i)
      a.raw[i] = static_cast<unsigned char>(
              std::min<int>(255,a.raw[i]+b.raw[i]));
   return a;
}