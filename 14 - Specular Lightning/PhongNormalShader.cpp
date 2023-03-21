#include "PhongNormalShader.h"
#include "TGAColor.h"

Vec3f PhongNormalShader::vertex(size_t face, size_t vert) {
   TexturedNormals::vertex(model, face, vert);
   return Shader::vertex(face,vert);
}

bool PhongNormalShader::fragment(Vec3f bary, TGAColor &color) const {
   TGAColor dColor = diffuseColor;
   auto n = TexturedNormals::fragment(light, bary, dColor);

   TGAColor sColor = specularColor;
   PhongNormals::specular(light,camera,n,specularPower,sColor);

   color = ambientColor * ambientWeight + dColor * diffuseWeight + sColor * specularWeight;
   return true;
}