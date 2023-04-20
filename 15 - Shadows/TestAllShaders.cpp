#include <vector>
#include <cmath>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include "Camera.h"
#include "Viewport.h"
#include "Triangle.h"

#include "ShadePhongShader.h"
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

vector<float> computeZbufferShader(ShadePhongShader *pShader, const Vec3f &light) {
    Camera camera{.eye = light, .center = {0, 0, 0}, .up = {0, 1, 0}};
    int shadowMapWidth = pShader->shadowMapWidth;
    int shadowMapHeight = pShader->shadowMapHeight;
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

int main() {

    string modelFileName = "../../obj/10_Capricorn.obj";
    Model model(modelFileName.c_str());
    model.normalize();

    Camera camera{.eye = {5, 2, 5}, .center = {0, 0, 0}, .up = {0, 1, 0}};
    Vec3f light = {300, 1000, 1000};
    Camera lightCamera{.eye = light, .center = {0, 0, 0}, .up = {0, 1, 0}};

    ShadePhongShader shadePhongShader;
    shadePhongShader.ambientColor = TGAColor(255, 255, 255, 255);
    shadePhongShader.diffuseColor = TGAColor(255, 128, 255, 255);
    shadePhongShader.specularColor = TGAColor(255, 255, 255, 255);
    shadePhongShader.ambientWeight = 0.1f;
    shadePhongShader.diffuseWeight = 1.f;
    shadePhongShader.specularWeight = 0.5f;
    shadePhongShader.specularPower = 10.f;
    shadePhongShader.camera = camera.direction();

    FlatShader flatShader;

    int const imageW = 800, imageH = 800;
    TGAImage image(imageW, imageH, TGAImage::RGB);
    vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());

    auto shader = &shadePhongShader;
    shader->model = &model;
    shader->view = camera.view();
    shader->projection = camera.projection();
    shader->viewport = make_viewport(0, 0, imageW, imageH);
    shader->light = light;
    shader->light.normalize();
    shader->transMatrix =   lightCamera.view() * lightCamera.projection();
    shader->shadowMapWidth = imageW;
    shader->shadowMapHeight = imageH;
    shader->shadowIntensity = 0.5f;
    shader->zbufferShade = computeZbufferShader(shader, light);

    render(shader, zbuffer, image);

    string filename = "result.tga";
    image.write_tga_file(filename.data());
}
