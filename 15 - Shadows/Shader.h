#ifndef CPPRENDERER_SHADER_H
#define CPPRENDERER_SHADER_H

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"
#include "Camera.h"

struct Shader {
   Vec3f light;
   Matrix4x4 view;
   Matrix4x4 projection;
   Matrix4x4 viewport;
   Model* model = nullptr;

   virtual ~Shader() = default;
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor& color) const = 0;
};

struct Culler {
   bool backfaceCulling = false;
   bool frontfaceCulling = false;
   Vec3f camera;
protected:
   bool isNotCulled(Vec3f normal) const;
};

struct TextureCoordinates {
   void vertex(Model* model, size_t face, size_t vert);
   TGAColor getValue(TGAImage* im, Vec3f bary) const;
protected:
   Vec3f varying_uv[3];
};

struct MonoColor {
   TGAColor color = TGAColor(255, 255, 255, 255);
   void fragment(TGAColor& color) const;
};

struct TexturedColor : public virtual TextureCoordinates {
   TGAImage* textureImage = nullptr;
   void fragment(Vec3f bary, TGAColor& color) const;
};

struct FlatNormals {
   void vertex(Model* model, Vec3f light, size_t face, size_t vert);
   Vec3f fragment(Vec3f bary, TGAColor& color) const; // returns normal
private:
   float intensity;
   Vec3f faceNormal;
};

struct GouraudNormals {
   void vertex(Model* model, Vec3f light, size_t face, size_t vert);
   void fragment(Vec3f bary, TGAColor& color) const;
private:
   Vec3f varying_intensity;
};

struct PhongNormals {
   void vertex(Model* model, size_t face, size_t vert);
   Vec3f fragment(Vec3f light, Vec3f bary, TGAColor& color) const;
   void specular(Vec3f light, Vec3f camera, Vec3f normal, float power, TGAColor& color) const;
private:
   Vec3f normals[3];
};

struct TexturedNormals : public virtual TextureCoordinates {
   TGAImage* normalsImage;
   Vec3f fragment( Vec3f light, Vec3f bary, TGAColor& color) const;
};

struct Shading {
    void vertex(Model* model, Matrix4x4 viewport, size_t face, size_t vert);
    Matrix4x4 transMatrix;
    std::vector<float> zbufferShade;
    int shadowMapWidth;
    int shadowMapHeight;
    float shadowIntensity = 0.5f;
    float specularShadowIntensity = 0.0f;
protected:
    void computeShade(Vec3f bary, float &shadowFactor, float &specularFactor) const;
private:
    Vec3f varying_shadowCoords[3];
};

void applyShade(TGAColor& color, float intensity);

#endif //CPPRENDERER_SHADER_H
