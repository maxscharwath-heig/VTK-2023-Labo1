#include <vector>
#include <array>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const int width  = 800;
const int height = 800;


std::array<Vec2i,2> boite_englobante(const TGAImage& image, Vec3f* t, int n = 3 ) {
   std::array<Vec2i,2> box { Vec2i{ image.get_width(), image.get_height() }, Vec2i{0, 0} };
   for(int i = 0; i < n; ++i) {
      if ( t[i].x < box[0].x ) box[0].x = t[i].x;
      if ( t[i].y < box[0].y ) box[0].y = t[i].y;
      if ( t[i].x > box[1].x ) box[1].x = t[i].x;
      if ( t[i].y > box[1].y ) box[1].y = t[i].y;
   }
   for(int i = 0; i < 2; ++i) {
      if(box[i].x < 0) box[i].x = 0;
      if(box[i].y < 0) box[i].y = 0;
      if(box[i].x >= image.get_width()) box[i].x = image.get_width()-1;
      if(box[i].y >= image.get_width()) box[i].y = image.get_height()-1;
   }
   return box;
}

Vec3f barycentriques(Vec3f* t, Vec3f p) {
   Vec3f x { (t[1].x - t[0].x),
             (t[2].x - t[0].x),
             (t[0].x - p.x) };
   Vec3f y { (t[1].y - t[0].y),
             (t[2].y - t[0].y),
             (t[0].y - p.y)};
   Vec3f u = x^y;
   if(abs(u.z) < 0.01)
      return Vec3f(-1,1,1);
   else
      return Vec3f { 1.f - (u.x+u.y)/u.z,u.x/u.z, u.y/u.z };
}

bool est_dans_le_triangle(Vec3f* t, Vec3f pt) {
   Vec3f b = barycentriques(t,pt);
   return ( b.x >= 0 and b.y >= 0 and b.z >= 0 );
}

void triangle(Vec3f *t, float* zbuffer, TGAImage &image, TGAColor color) {
   auto bbox = boite_englobante(image, t);
   for(int y = bbox[0].y; y <= bbox[1].y; ++y) {
      for(int x = bbox[0].x; x <= bbox[1].x; ++x) {
         Vec3f p { float(x), float(y), 0};
         Vec3f b = barycentriques(t,p);
         if ( b.x >= 0 and b.y >= 0 and b.z >= 0 ) {
            p.z = b.x*t[0].z + b.y*t[1].z + b.z*t[2].z;
            if(zbuffer[x + image.get_width() * y] < p.z) {
               zbuffer[x + image.get_width() * y] = p.z;
               image.set(p.x,p.y,color);
            }
         }

      }
   }
}

int main(int argc, char** argv) {

   Model *model = NULL;
   if (2==argc) {
      model = new Model(argv[1]);
   } else {
      model = new Model("../../obj/african_head.obj");
//        model = new Model("../../obj/reconstructed_head.obj");
   }

   const Vec3f light(0,0,-1);

   TGAImage image(width, height, TGAImage::RGB);
   float zbuffer[width*height];
   for(unsigned i = 0; i < width*height; ++i)
      zbuffer[i] = std::numeric_limits<float>::lowest();

   for (int i=0; i<model->nfaces(); i++) {
      std::vector<int> face = model->face(i);

      Vec3f screen[3];
      Vec3f world[3];

      for (int j=0; j<3; j++) {
         world[j] = model->vert(face[j]);
//            world[j] = model->vert(face[j]) * 0.005;
         screen[j] = Vec3f((world[j] .x + 1.) * width / 2.,
                           (world[j].y+1.)*height/2., world[j].z);
      }

      Vec3f n = (world[2]-world[0])^(world[1]-world[0]);
      n.normalize();
      float I = n * light;

      if(I>=0) {
         TGAColor color(I * 255, I * 255, I * 255, 255);
         triangle(screen, zbuffer, image, color);
      }
   }

   image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
   image.write_tga_file("zbuffering.tga");
   delete model;
   return 0;
}

