#include <vector>
#include <cmath>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include "Camera.h"
#include "Viewport.h"
#include "Triangle.h"

#include "PhongTextureShader.h"
#include "ShadePhongTextureShader.h"
#include "FlatShader.h"

using namespace std;

void render(Shader *shader, vector<float> &zbuffer, TGAImage &image) {
    for (size_t f = 0; f < size_t(shader->model->nfaces()); ++f) {
        Vec3f screen[3];
        for (size_t v = 0; v < 3; ++v) {
            screen[v] = shader->vertex(f, v);
        }
        triangle(screen, *shader, zbuffer.data(), image);
    }
}

vector<float> computeZbufferShader(Shader *pShader, const Vec3f &light, int shadowMapWidth, int shadowMapHeight) {
    Camera camera{.eye = light, .center = {0, 0, 0}, .up = {0, 1, 0}};
    TGAImage image(shadowMapWidth, shadowMapHeight, TGAImage::RGB);
    vector<float> zbuffer(shadowMapWidth * shadowMapHeight, std::numeric_limits<float>::lowest());
    FlatShader shader;
    shader.model = pShader->model;
    shader.view = camera.view();
    shader.projection = camera.projection();
    shader.viewport = pShader->viewport;
    shader.light = light;
    shader.light.normalize();
    render(&shader, zbuffer, image);
    return zbuffer;
}

/**
 * VTK - Labo 1 - Ombrages
 * @authors Nicolas Crausaz & Maxime Scharwath
 */
int main() {

    string modelFileName = "../../obj/diablo3_pose.obj";
    Model model(modelFileName.c_str());
    model.normalize();

    string textureFileName = "../../obj/diablo3_pose_diffuse.tga";
    TGAImage textureImage;
    textureImage.read_tga_file(textureFileName);

    Camera camera{.eye = {2, 0, 4}, .center = {0, 0, 0}, .up = {0, 1, 0}};
    Vec3f light = {300, 1000, 1000};
    Camera lightCamera{.eye = light, .center = {0, 0, 0}, .up = {0, 1, 0}};

    ShadePhongTextureShader shadeTexturePhongShader;
    shadeTexturePhongShader.ambientWeight = 0.2f;
    shadeTexturePhongShader.diffuseWeight = 0.8f;
    shadeTexturePhongShader.specularWeight = 0.5f;
    shadeTexturePhongShader.specularPower = 10.f;
    shadeTexturePhongShader.camera = camera.direction();
    shadeTexturePhongShader.textureImage = &textureImage;

    PhongTextureShader phongTextureShader;
    phongTextureShader.ambientWeight = 0.2f;
    phongTextureShader.diffuseWeight = 0.8f;
    phongTextureShader.specularWeight = 0.5f;
    phongTextureShader.specularPower = 10.f;
    phongTextureShader.camera = camera.direction();
    phongTextureShader.textureImage = &textureImage;

    FlatShader flatShader;

    int const imageW = 2000;
    int const imageH = 1000;
    TGAImage image(imageW, imageH, TGAImage::RGB);
    vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());

    // Create a vector of shader pointers
    vector<Shader *> shaders = {&shadeTexturePhongShader, &phongTextureShader};

    // Render each shader in the desired position
    for (size_t i = 0; i < shaders.size(); ++i) {
        fill(zbuffer.begin(),zbuffer.end(),std::numeric_limits<float>::lowest());
        Shader *currentShader = shaders[i];
        currentShader->model = &model;
        currentShader->view = camera.view();
        currentShader->projection = camera.projection();
        currentShader->viewport = make_viewport(imageW / 2 * i, 0, imageW / 2, imageH, 0.5);
        currentShader->light = light;
        currentShader->light.normalize();
        if(currentShader == &shadeTexturePhongShader){
            auto *shadedShader = (ShadePhongTextureShader *) currentShader;
            shadedShader->transMatrix = lightCamera.view() * lightCamera.projection();
            shadedShader->shadowMapWidth = imageW;
            shadedShader->shadowMapHeight = imageH;
            shadedShader->shadowIntensity = 0.5f;
            shadedShader->specularShadowIntensity = 0.0f;
            shadedShader->zbufferShade = computeZbufferShader(shadedShader, light, imageW, imageH);
        }
        render(currentShader, zbuffer, image);
    }

    string filename = "result_texture.tga";
    image.write_tga_file(filename.data());
}
