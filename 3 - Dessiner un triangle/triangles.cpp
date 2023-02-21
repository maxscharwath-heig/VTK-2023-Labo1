#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const int width  = 200;
const int height = 200;

void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

void solid_angle(Vec2i t0, Vec2i t1, Vec2i t2, Vec2i t3, TGAImage &image, TGAColor color) {
   if(t0.y == t1.y)
   {
      if(t0.x > t1.x) std::swap(t0,t1);
      for(int x = t0.x; x < t1.x; ++x)
         image.set(x,t0.y,color);
   }

   for(int y = t0.y; y <= t1.y; ++y)
   {
      float t01 = ( y - t0.y ) / float( t1.y - t0.y );
      int x01 = t0.x + t01 * (t1.x - t0.x);

      float t23 = ( y - t2.y ) / float( t3.y - t2.y );
      int x23 = t2.x + t23 * (t3.x - t2.x);

      if(x23 < x01) std::swap(x01,x23);
      for(int x = x01; x <= x23; ++x)
         image.set(x,y,color);
   }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
   if(t1.y < t0.y) std::swap(t0,t1);
   if(t2.y < t0.y) std::swap(t0,t2);
   if(t2.y < t1.y) std::swap(t1,t2);

   solid_angle(t0, t1, t0, t2, image, color);
   solid_angle(t1, t2, t0, t2, image, color);
}

int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);


    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("triangle.tga");
    return 0;
}

