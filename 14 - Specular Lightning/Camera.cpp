#include "Camera.h"

float Camera::focalDistance() const {
   return (eye-center).norm();
}

Vec3f Camera::direction() const {
   return (eye-center).normalize();
}

Matrix4x4 Camera::view() const {
   Matrix4x4 m = Matrix4x4::identity();

   Vec3f z = (eye-center).normalize();
   Vec3f x = (up^z).normalize();
   Vec3f y = (z^x).normalize();
   for (size_t i=0; i<3; i++) {
      m[0][i] = x.raw[i];
      m[1][i] = y.raw[i];
      m[2][i] = z.raw[i];
      m[i][3] = -center.raw[i];
   }
   return m;
}

Matrix4x4 Camera::projection() const {
   Matrix4x4 m = Matrix4x4::identity();
   m[3][2] = -1.f / focalDistance();
   return m;
}