#include "PhongNormalTextureShader.h"
#include "TGAColor.h"

Vec3f PhongNormalTextureShader::vertex(size_t face, size_t vert) {
   TexturedNormals::vertex(model, face, vert);
   TexturedColor::vertex(model, face, vert);
   return Shader::vertex(face,vert);
}

bool PhongNormalTextureShader::fragment(Vec3f bary, TGAColor &color) const {
   TexturedColor::fragment(bary,color);

   TGAColor dColor = color;
   auto n = TexturedNormals::fragment(light, bary, dColor);

   TGAColor sColor = color;
   PhongNormals::specular(light,camera,n,specularPower,sColor);

   color = color * ambientWeight + dColor * diffuseWeight + sColor * specularWeight;
   return true;
}