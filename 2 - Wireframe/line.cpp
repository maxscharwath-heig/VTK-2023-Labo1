#include "line.h"
#include <cmath>        // std::abs
#include <utility>        // std::swap

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
   bool steep = false;
   if (std::abs(x0-x1)<std::abs(y0-y1)) { // if the line is steep, we transpose the image
      std::swap(x0, y0);
      std::swap(x1, y1);
      steep = true;
   }
   if (x0>x1) { // make it left−to−right
      std::swap(x0, x1);
      std::swap(y0, y1);
   }
   int dx = (x1-x0);
   int ystep = std::abs(2*(y1-y0));
   int dysign = (y1>y0) ? +1:-1;
   int yi = y0;
   int yp = -dx; // y = yi + dysign * float(yp+dx)/(2*dx)
   for (int x=x0; x<=x1; x++) {
      yp += ystep;
      if(yp > 0) {
         yp -= 2*dx;
         yi += dysign;
      }
      if (steep) image.set(yi, x, color);
      else       image.set(x, yi, color);
   }
}