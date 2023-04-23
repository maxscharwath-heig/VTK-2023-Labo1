#include "ShadePhongShader.h"
#include "TGAColor.h"

Vec3f ShadePhongShader::vertex(size_t face, size_t vert) {
    Vec3f world = model->vert(size_t(model->face(face)[vert]));
    shadowCoords[vert] =  viewport * transMatrix * VecH(world);
    PhongNormals::vertex(model, face, vert);
    return  Shader::vertex(face, vert);
}

bool ShadePhongShader::fragment(Vec3f bary, TGAColor &color) const {
    Vec3f shadow = shadowCoords[0] * bary.x + shadowCoords[1] * bary.y + shadowCoords[2] * bary.z;

    int index = int(shadow.x) + int(shadow.y) * shadowMapWidth;

    TGAColor dColor = diffuseColor;
    auto n = PhongNormals::fragment(light, bary, dColor);

    TGAColor sColor = specularColor;
    PhongNormals::specular(light, camera, n, specularPower, sColor);

    TGAColor ambientComponent = ambientColor * ambientWeight;
    TGAColor diffuseComponent = dColor * diffuseWeight;
    TGAColor specularComponent = sColor * specularWeight;

    float shadowFactor = 1.0f;
    float specularFactor = 1.0f;
    if (index >= 0 && index < shadowMapWidth * shadowMapHeight) {
        float z = zbufferShade[index];
        float bias = 10;
        if (z > shadow.z + bias) {
            shadowFactor = shadowIntensity;
            specularFactor = specularShadowIntensity;
        }
    }
    color = ambientComponent + diffuseComponent * shadowFactor + specularComponent * specularFactor;
    return true;
}
