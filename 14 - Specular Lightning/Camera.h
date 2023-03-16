#ifndef CPPRENDERER_CAMERA_H
#define CPPRENDERER_CAMERA_H

#include "geometry.h"

struct Camera {
   Vec3f eye, center, up;
   float focalDistance() const;
   Vec3f direction() const;
   Matrix4x4 view() const;
   Matrix4x4 projection() const;
};

#endif //CPPRENDERER_CAMERA_H
