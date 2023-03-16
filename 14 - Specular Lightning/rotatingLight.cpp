#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include "Camera.h"
#include "Viewport.h"
#include "Triangle.h"

#include "NormalTextureShader.h"
#include "GouraudTextureShader.h"

using namespace std;

int main() {

   string modelFileName = "../../../ocuisenaire-CPP-Renderer-2023/obj/african_head.obj";
   Model model(modelFileName.c_str());

   string textureFileName = "../../../ocuisenaire-CPP-Renderer-2023/obj/african_head_diffuse.tga";
   TGAImage texture;
   texture.read_tga_file(textureFileName.c_str());
   texture.flip_vertically();

   string normalsFileName = "../../../ocuisenaire-CPP-Renderer-2023/obj/african_head_nm.tga";
   TGAImage normals;
   normals.read_tga_file(normalsFileName.c_str());
   normals.flip_vertically();

   Camera camera{.eye = {1, 1, 2}, .center = {0, 0, 0}, .up = {0, 1, 0}};

   NormalTextureShader normalTextureShader;
   normalTextureShader.texture = &texture;
   normalTextureShader.normals = &normals;

   int const nFrames = 100;
   for(int i = 0; i < nFrames; ++i) {
      float angle = i * 2 * M_PI / nFrames;

      int const imageW = 800, imageH = 800;
      int const viewW = imageW, viewH = imageH;
      TGAImage image(imageW, imageH, TGAImage::RGB);
      vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());

      fill(zbuffer.begin(), zbuffer.end(), std::numeric_limits<float>::lowest());

      normalTextureShader.model = &model;
      normalTextureShader.view = camera.view();
      normalTextureShader.projection = camera.projection();
      normalTextureShader.viewport = make_viewport(0, 0, viewW, viewH);
      normalTextureShader.light = {sin(angle), 0, cos(angle)};

      for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
         Vec3f screen[3];
         for (size_t v = 0; v < 3; ++v) {
            screen[v] = normalTextureShader.vertex(f, v);
         }
         triangle(screen, normalTextureShader, zbuffer.data(), image);
      }

      image.flip_vertically();
      string filename = "rotatingLight"+to_string(1000+i)+".tga";
      image.write_tga_file(filename.data());
   }
}

