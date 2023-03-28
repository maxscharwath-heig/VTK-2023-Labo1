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
#include "PhongShader.h"
#include "PhongTextureShader.h"
#include "PhongNormalShader.h"
#include "PhongNormalTextureShader.h"

using namespace std;

int main() {

   string modelFileName = "../../obj/african_head.obj";
   Model model(modelFileName.c_str());

   string textureFileName = "../../obj/african_head_diffuse.tga";
   TGAImage texture;
   texture.read_tga_file(textureFileName.c_str());

   string normalsFileName = "../../obj/african_head_nm.tga";
   TGAImage normals;
   normals.read_tga_file(normalsFileName.c_str());

   Camera camera{ .eye = {1,1,2}, .center = {0,0,0}, .up = {0,1,0} };

   FlatShader flatShader;
   flatShader.color = TGAColor(255,128,128,255);

   FlatTextureShader flatTextureShader;
   flatTextureShader.textureImage = &texture;

   GouraudShader gouraudShader;
   gouraudShader.color = TGAColor(128,255,128,255);

   GouraudTextureShader gouraudTextureShader;
   gouraudTextureShader.textureImage = &texture;

   NormalShader normalShader;
   normalShader.color =  TGAColor(128,128,255,255);
   normalShader.normalsImage = &normals;

   NormalTextureShader normalTextureShader;
   normalTextureShader.textureImage = &texture;
   normalTextureShader.normalsImage = &normals;

   PhongShader phongShader;
   phongShader.ambientColor = TGAColor(255,255,255,255);
   phongShader.diffuseColor = TGAColor(255,128,255,255);
   phongShader.specularColor = TGAColor(255,255,255,255);
   phongShader.ambientWeight = 0.1f;
   phongShader.diffuseWeight = 1.f;
   phongShader.specularWeight = 0.5f;
   phongShader.specularPower = 10.f;
   phongShader.camera = camera.direction();

   PhongTextureShader phongTextureShader;
   phongTextureShader.textureImage = &texture;
   phongTextureShader.ambientWeight = 0.1f;
   phongTextureShader.diffuseWeight = 1.f;
   phongTextureShader.specularWeight = 1.f;
   phongTextureShader.specularPower = 10.f;
   phongTextureShader.camera = camera.direction();

   PhongNormalShader phongNormalShader;
   phongNormalShader.normalsImage = &normals;
   phongNormalShader.ambientColor = TGAColor(255,255,255,255);
   phongNormalShader.diffuseColor = TGAColor(255,128,255,255);
   phongNormalShader.specularColor = TGAColor(255,255,255,255);
   phongNormalShader.ambientWeight = 0.1f;
   phongNormalShader.diffuseWeight = 1.f;
   phongNormalShader.specularWeight = 0.5f;
   phongNormalShader.specularPower = 10.f;
   phongNormalShader.camera = camera.direction();

   PhongNormalTextureShader phongNormalTextureShader;
   phongNormalTextureShader.normalsImage = &normals;
   phongNormalTextureShader.textureImage = &texture;
   phongNormalTextureShader.ambientWeight = 0.1f;
   phongNormalTextureShader.diffuseWeight = 1.f;
   phongNormalTextureShader.specularWeight = 1.f;
   phongNormalTextureShader.specularPower = 10.f;
   phongNormalTextureShader.camera = camera.direction();

   vector<Shader*> shaders = {
           &flatTextureShader,
           &gouraudTextureShader,
           &normalTextureShader,
           &phongTextureShader,
           &phongNormalTextureShader,
           &flatShader,
           &gouraudShader,
           &normalShader,
           &phongShader,
           &phongNormalShader};

   int const imageW = 2500, imageH = 1000;
   int const gridX = 5, gridY = 2;
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
      shader->light = {0,0,1};
      shader->light.normalize();

      for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
         Vec3f screen[3];
         for (size_t v = 0; v < 3; ++v) {
            screen[v] = shader->vertex(f, v);
         }
         triangle(screen, *shader, zbuffer.data(), image);
      }
   }

   string filename = "allShaders.tga";
   image.write_tga_file(filename.data());
}

