#include "ShadeTexturePhongShader.h"
#include "TGAColor.h"

Vec3f ShadeTexturePhongShader::vertex(size_t face, size_t vert) {
    PhongNormals::vertex(model, face, vert);
    TexturedColor::vertex(model, face, vert);
    Shading::vertex(model, viewport, face, vert);
    return Shader::vertex(face, vert);
}

bool ShadeTexturePhongShader::fragment(Vec3f bary, TGAColor &color) const {
    TexturedColor::fragment(bary,color);

    TGAColor dColor = color;
    auto n = PhongNormals::fragment(light, bary, dColor);

    TGAColor sColor = color;
    PhongNormals::specular(light,camera,n,specularPower,sColor);

    float shadowFactor = 0.f;
    float specularFactor = 0.f;
    Shading::computeShade(bary, shadowFactor, specularFactor);

    color = color * ambientWeight + dColor * diffuseWeight * shadowFactor + sColor * specularWeight * specularFactor;
    return true;
}

