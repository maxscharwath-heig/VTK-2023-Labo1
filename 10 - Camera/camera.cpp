#include <iostream>
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

void triangle(Vec3f t[3], Vec3f tc[3], float* zbuffer, TGAImage &image, TGAImage &texture, Vec3f intensity) {
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

                    float gouraud = intensity * b;
                    if(gouraud < 0) gouraud = 0;

                    for(int i = 0; i < 3; ++i) color.raw[i] *= gouraud;

                    image.set(p.x,p.y,color);
                }
            }

        }
    }
}

Matrix4x4 make_translate(Vec3f v) {
    Matrix4x4 m = Matrix4x4::identity();
    for(int i=0; i < 3; ++i)
        m[i][3] = v.raw[i];
    return m;
}

Matrix4x4 make_scale(Vec3f v) {
    Matrix4x4 m = Matrix4x4::identity();
    for(int i=0; i < 3; ++i)
        m[i][i] = v.raw[i];
    return m;
}

Matrix4x4 make_viewport(int x, int y, size_t w, size_t h) {
    Matrix4x4 t = make_translate(Vec3f{x+w/2.f, y+w/2.f, 100});
    Matrix4x4 s = make_scale(Vec3f{w/2.f, h/2.f, 100});
    return t*s;
}

VecH xyzw(Vec3f v) {
    VecH m(1.f);
    for(size_t i = 0; i < 3; ++i)
        m[i][0] = v.raw[i];
    return m;
}

Vec3f xyz(VecH m) {
    Vec3f v;
    for(size_t i = 0; i < 3; ++i)
        v.raw[i] = m[i][0]/m[3][0];
    return v;
}

int main(int argc, char** argv) {
    string modelFileName = "../../obj/african_head.obj";
    string textureFileName = "../../obj/african_head_diffuse.tga";

    Model model(modelFileName.c_str());
    TGAImage texture;
    texture.read_tga_file(textureFileName.c_str());
    texture.flip_vertically();

    const Vec3f light(0,0,-1);

    TGAImage image(width, height, TGAImage::RGB);
    float zbuffer[width * height];
    for (unsigned i = 0; i < width * height; ++i)
        zbuffer[i] = std::numeric_limits<float>::lowest();

    Matrix4x4 viewport = make_viewport(0, 0, width, height);
    viewport = make_viewport(0, 0, 800, 800);

    Vec3f camera{0.f, 0.f, 2.f};

    Matrix4x4 projection = Matrix4x4::identity();
    projection[3][2] = -1.f / camera.z;

    for (int i = 0; i < model.nfaces(); i++) {
        Vec3f screen[3];
        Vec3f world[3];
        Vec3f text[3];
        Vec3f intensity;

        for (int j = 0; j < 3; j++) {
            world[j] = model.vert(model.face(i)[j]);
            screen[j] = xyz(viewport * projection * xyzw(world[j]));
            text[j] = model.texture(model.face_texts(i)[j]);
            Vec3f n = model.normal(model.face_normals(i)[j]) * -1;
            intensity.raw[j] = n * light;
        }
        triangle(screen, text, zbuffer, image, texture, intensity);
    }

    image.flip_vertically();
    string filename = "camera.tga";
    image.write_tga_file(filename.data());

    return 0;
}

