#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<Vec3f> texts_;
    std::vector<Vec3f> norms_;
    std::vector<std::vector<int> > faces_;
    std::vector<std::vector<int> > face_texts_;
    std::vector<std::vector<int> > face_norms_;
public:
	Model(const char *filename);
	~Model();
	int nverts() const;
	int nfaces() const;
    const Vec3f& vert(size_t i) const;
    const Vec3f& texture(size_t i) const;
    const Vec3f& normal(size_t i) const;
    const std::vector<int>& face(size_t idx) const;
    const std::vector<int>& face_texts(size_t idx) const;
    const std::vector<int>& face_normals(size_t idx) const;
};

#endif //__MODEL_H__
