#ifndef CPPRENDERER_SHADEPHONGSHADER_H
#define CPPRENDERER_SHADEPHONGSHADER_H


#include "Shader.h"

struct ShadePhongShader
        : public Shader,
          public Culler,
          public Shading,
          public PhongNormals {
    TGAColor ambientColor, diffuseColor, specularColor;
    float ambientWeight = 0.3f, diffuseWeight = 0.7f, specularWeight = 0.f;
    float specularPower = 10.f;
    virtual Vec3f vertex(size_t face, size_t vert);
    virtual bool fragment(Vec3f bary, TGAColor &color) const;
};


#endif //CPPRENDERER_SHADEPHONGSHADER_H
