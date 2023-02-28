#include <vector>
#include <string>
#include <array>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

using namespace std;

const int width  = 800;
const int height = 800;

template<class Vec, typename Comp>
void compare_coordinates(Vec2i& p1, const Vec& p2, Comp compare) {
    if(compare(p2.x,p1.x)) p1.x = p2.x;
    if(compare(p2.y,p1.y)) p1.y = p2.y;
}
template<class Vec>
void smallest_coordinates(Vec2i& p1, const Vec& p2) {
    compare_coordinates(p1,p2,less<int>());
}
template<class Vec>
void largest_coordinates(Vec2i& p1, const Vec& p2) {
    compare_coordinates(p1,p2,greater<int>());
}
std::array<Vec2i,2> boite_englobante(const TGAImage& image, Vec3f* t, int n = 3 )
{
    Vec2i t0 (t[0].x, t[0].y );
    std::array<Vec2i,2> box { t0, t0 } ;
    for(int i = 1; i < n; ++i) {
        smallest_coordinates(box[0],t[i]);
        largest_coordinates(box[1],t[i]);
    }

    const Vec2i imageMin { 0, 0 };
    const Vec2i imageMax { image.get_width()-1, image.get_height() - 1 };
    for(int i = 0; i < 2; ++i) {
        largest_coordinates(box[i], imageMin);
        smallest_coordinates(box[i], imageMax);
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

void triangle(Vec3f *t, Vec3f* tc, float* zbuffer, TGAImage &image, TGAImage &texture, float intensity) {
    auto bbox = boite_englobante(image, t);
    for(int y = bbox[0].y; y <= bbox[1].y; ++y) {
        for(int x = bbox[0].x; x <= bbox[1].x; ++x) {
            Vec3f p { float(x), float(y), 0};
            Vec3f b = barycentriques(t,p);
            if ( b.x >= 0 and b.y >= 0 and b.z >= 0 ) {
                p.z = b.x*t[0].z + b.y*t[1].z + b.z*t[2].z;
                if(zbuffer[x + image.get_width() * y] < p.z) {
                    zbuffer[x + image.get_width() * y] = p.z;

                    Vec3f ptc = tc[0]*b.x + tc[1]*b.y + tc[2]*b.z;
                    Vec2i pc ( ptc.x * texture.get_width() , ptc.y * texture.get_height() );
                    TGAColor color = texture.get(pc.x, pc.y);
                    for(int i = 0; i < 3; ++i) color.raw[i] *= intensity;

                    image.set(x,y,color);
                }
            }

        }
    }
}

int main(int argc, char** argv) {

    string modelFileName = "../../obj/african_head.obj";
    Model model(modelFileName.c_str());

    string textureFileName = "../../obj/african_head_diffuse.tga";
    TGAImage texture;
    texture.read_tga_file(textureFileName.c_str());
    texture.flip_vertically();

    const Vec3f light(0,0,-1);

    TGAImage image(width, height, TGAImage::RGB);

    float zbuffer[width*height];
    for(unsigned i = 0; i < width*height; ++i)
        zbuffer[i] = std::numeric_limits<float>::lowest();

    for (int i=0; i<model.nfaces(); i++) {
        Vec3f screen[3];
        Vec3f world[3];
        Vec3f text[3];

        for (int j=0; j<3; j++) {
            world[j] = model.vert(model.face(i)[j]);
            screen[j] = Vec3f((world[j] .x + 1.) * width / 2.,
                    (world[j].y+1.)*height/2., world[j].z);
            text[j] = model.texture(model.face_texts(i)[j]);
        }

        Vec3f n = (world[2]-world[0])^(world[1]-world[0]);
        n.normalize();
        float I = n * light;

        if(I>=0) {
            triangle(screen, text, zbuffer, image, texture, I);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("gouraud.tga");
    return 0;
}

