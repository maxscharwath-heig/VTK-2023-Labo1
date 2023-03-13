#include "Viewport.h"
#include <algorithm> // std::min

Matrix4x4 make_translate(Vec3f v) {
   Matrix4x4 m = Matrix4x4::identity();
   for(size_t i=0; i < 3; ++i)
      m[i][3] = v.raw[i];
   return m;
}

Matrix4x4 make_scale(float s) {
   Matrix4x4 m = Matrix4x4::identity();
   for(size_t i=0; i < 3; ++i)
      m[i][i] = s;
   return m;
}

Matrix4x4 make_viewport(int x, int y, size_t w, size_t h, float zoom) {
   Matrix4x4 m = make_translate(Vec3f{float(x)+w/2,float(y)+h/2,0.f});
   m *= make_scale(std::min<float>(w, h));
   m *= make_scale(zoom);
   return m;
}