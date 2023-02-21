#include <vector>
#include <cmath>
#include <array>
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
    TGAImage image(width, height, TGAImage::RGB);

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    triangle(t0, image, red);
    triangle(t1, image, white);
    triangle(t2, image, green);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("gpu.tga");
    return 0;
}

