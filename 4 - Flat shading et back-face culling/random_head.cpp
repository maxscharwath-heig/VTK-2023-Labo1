#include <vector>
#include <array>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

Model *model = NULL;
const int width  = 800;
const int height = 800;


std::array<Vec2i,2> boite_englobante(const TGAImage& image, Vec2i* t, int n = 3 ) {
    std::array<Vec2i,2> box { Vec2i{image.get_width()-1, image.get_height()-1} , Vec2i{0,0} };
    for(int i = 0; i < n; ++i) {
        if ( t[i].x < box[0].x ) box[0].x = t[i].x;
        if ( t[i].y < box[0].y ) box[0].y = t[i].y;
        if ( t[i].x > box[1].x ) box[1].x = t[i].x;
        if ( t[i].y > box[1].y ) box[1].y = t[i].y;
    }
    return box;
}

Vec3f barycentriques(Vec2i* t, Vec2i p) {
    Vec3f x { static_cast<float>(t[1].x - t[0].x),
              static_cast<float>(t[2].x - t[0].x),
              static_cast<float>(t[0].x - p.x) };
    Vec3f y { static_cast<float>(t[1].y - t[0].y),
              static_cast<float>(t[2].y - t[0].y),
              static_cast<float>(t[0].y - p.y)};
    Vec3f u = x^y;
    if(abs(u.z) < 1)
        return Vec3f(-1,1,1);
    else
        return Vec3f { 1.f - (u.x+u.y)/u.z,u.x/u.z, u.y/u.z };
}

bool est_dans_le_triangle(Vec2i* t, Vec2i pt) {
    Vec3f b = barycentriques(t,pt);
    return ( b.x >= 0 and b.y >= 0 and b.z >= 0 );
}

void triangle(Vec2i *t, TGAImage &image, TGAColor color) {
    auto bbox = boite_englobante(image, t);
    for(Vec2i p { 0, bbox[0].y }; p.y <= bbox[1].y; ++p.y) {
        for(p.x = bbox[0].x; p.x <= bbox[1].x; ++p.x) {
            if (est_dans_le_triangle(t, p))
                image.set(p.x,p.y,color);
        }
    }
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../../obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);

        Vec2i t[3];
        for (int j=0; j<3; j++) {
            Vec3f world_coords = model->vert(face[j]);
            t[j] = Vec2i((world_coords.x+1.)*width/2., (world_coords.y+1.)*height/2.);
        }

        triangle(t, image, TGAColor(rand() % 256, rand() % 256, rand() % 256, 255));
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("random.tga");
    delete model;
    return 0;
}

