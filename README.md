# Cpp Renderer 2023

Réalisation d'un mini OpenGL en C++ dans le cadre du cours VTK de la HEIG-VD

# Labo 1 - Ombrages

> Nicolas Crausaz & Maxime Scharwath

## Indications

Nous avons réaliser une version d'ombrage sur Phong: `Labo1_Ombrages_main.cpp`.
ainsi que la version "texturisée": `/Labo1_Ombrages_main_textured.cpp`

Dans les deux cas, les version avec et sans ombrages s'affichent côte à côte.

Au niveau de l'implémentation, nous avons principalement travaillé dans les fichiers 
- `ShadePhongShader` (Phong avec ombrages) 
- `ShadePhongTextureShader` (Phong + texture avec ombrages)
- Nous avons ajouté la structure `Shading` (dans Shader.h) et les méthod‡es chargées d'appliquer la logique d'ombrage dans Shader.cpp