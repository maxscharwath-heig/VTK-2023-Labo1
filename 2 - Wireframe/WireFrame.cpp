#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "line.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../../obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);

    // A compléter

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("wireframe.tga");
    delete model;
}

