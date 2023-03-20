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

#include "FlatShader.h"
#include "FlatTextureShader.h"
#include "GouraudShader.h"
#include "GouraudTextureShader.h"
#include "NormalShader.h"
#include "NormalTextureShader.h"

using namespace std;

int main() {

   string modelFileName = "../../obj/african_head.obj";
   Model model(modelFileName.c_str());

   string textureFileName = "../../obj/african_head_diffuse.tga";
   TGAImage texture;
   texture.read_tga_file(textureFileName.c_str());
   texture.flip_vertically();

   string normalsFileName = "../../obj/african_head_nm.tga";
   TGAImage normals;
   normals.read_tga_file(normalsFileName.c_str());
   normals.flip_vertically();

   Camera camera{ .eye = {1,1,2}, .center = {0,0,0}, .up = {0,1,0} };

   FlatShader flatShader;
   flatShader.color = TGAColor(255,128,128,255);

   FlatTextureShader flatTextureShader;
   flatTextureShader.texture = &texture;

   GouraudShader gouraudShader;
   gouraudShader.color = TGAColor(128,255,128,255);

   GouraudTextureShader gouraudTextureShader;
   gouraudTextureShader.texture = &texture;

   NormalShader normalShader;
   normalShader.color =  TGAColor(128,128,255,255);
   normalShader.normals = &normals;

   NormalTextureShader normalTextureShader;
   normalTextureShader.texture = &texture;
   normalTextureShader.normals = &normals;

   vector<Shader*> shaders = { &flatShader,
                               &flatTextureShader,
                               &gouraudShader,
                               &gouraudTextureShader,
                               &normalShader,
                               &normalTextureShader };

   int const imageW = 1500, imageH = 1000;
   int const gridX = 3, gridY = 2;
   int const viewW = imageW / gridX, viewH = imageH / gridY;
   TGAImage image(imageW, imageH, TGAImage::RGB);
   vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());

   for(size_t i = 0; i < shaders.size(); ++i) {
      fill(zbuffer.begin(),zbuffer.end(),std::numeric_limits<float>::lowest());

      int x = int(i) % gridX;
      int y = int(i) / gridX;

      auto& shader = shaders[i];
      shader->model = &model;
      shader->view = camera.view();
      shader->projection = camera.projection();
      shader->viewport = make_viewport(x * viewW, y * viewH, viewW, viewH);
      shader->light = {0, 0, 1};

      for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
         Vec3f screen[3];
         for (size_t v = 0; v < 3; ++v) {
            screen[v] = shader->vertex(f, v);
         }
         triangle(screen, *shader, zbuffer.data(), image);
      }
   }

   image.flip_vertically();
   string filename = "allShaders.tga";
   image.write_tga_file(filename.data());
}

