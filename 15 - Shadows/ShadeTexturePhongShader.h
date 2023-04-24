//
// Created by Maxime Scharwath on 24.04.23.
//

#ifndef CPPRENDERER_SHADETEXTUREPHONGSHADER_H
#define CPPRENDERER_SHADETEXTUREPHONGSHADER_H

#include "Shader.h"

struct ShadeTexturePhongShader
        : public Shader,
          public TexturedColor,
          public Culler,
          public Shading,
          public PhongNormals {
    float ambientWeight = 0.3f, diffuseWeight = 0.7f, specularWeight = 0.f;
    float specularPower = 10.f;
    virtual Vec3f vertex(size_t face, size_t vert);
    virtual bool fragment(Vec3f bary, TGAColor &color) const;
};


#endif //CPPRENDERER_SHADETEXTUREPHONGSHADER_H
