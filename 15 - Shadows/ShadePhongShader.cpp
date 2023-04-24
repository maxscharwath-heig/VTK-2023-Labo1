#include "ShadePhongShader.h"
#include "TGAColor.h"

Vec3f ShadePhongShader::vertex(size_t face, size_t vert) {
    PhongNormals::vertex(model, face, vert);
    Shading::vertex(model, viewport, face, vert);
    return Shader::vertex(face, vert);
}

bool ShadePhongShader::fragment(Vec3f bary, TGAColor &color) const {
    TGAColor dColor = diffuseColor;
    Vec3f n = PhongNormals::fragment(light, bary, dColor);

    TGAColor sColor = specularColor;
    PhongNormals::specular(light, camera, n, specularPower, sColor);

    float shadowFactor = 0.f;
    float specularFactor = 0.f;
    Shading::computeShade(bary, shadowFactor, specularFactor);

    color = ambientColor * ambientWeight + dColor * diffuseWeight * shadowFactor + sColor * specularWeight * specularFactor;
    return true;
}
