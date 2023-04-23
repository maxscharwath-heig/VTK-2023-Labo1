#ifndef CPPRENDERER_SHADEPHONGSHADER_H
#define CPPRENDERER_SHADEPHONGSHADER_H

#include "PhongShader.h"

struct ShadePhongShader : public PhongShader {
    Matrix4x4 transMatrix;
    std::vector<float> zbufferShade;
    Vec3f shadowCoords[3];
    int shadowMapWidth, shadowMapHeight;
    float shadowIntensity = 0.5f;
    float specularShadowIntensity = 0.0f;

    Vec3f vertex(size_t face, size_t vert) override;

    bool fragment(Vec3f bary, TGAColor &color) const override;
};


#endif //CPPRENDERER_SHADEPHONGSHADER_H
