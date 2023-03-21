#ifndef CPPRENDERER_TGACOLOR_H
#define CPPRENDERER_TGACOLOR_H

#include "tgaimage.h"

TGAColor operator*(TGAColor c, float f);
TGAColor operator*(float f, TGAColor const &c);
TGAColor operator+(TGAColor a, TGAColor const& b);

#endif //CPPRENDERER_TGACOLOR_H
