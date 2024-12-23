#pragma once
#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
	std::vector<Vec3f> verts_;
	std::vector<int> faces;
	std::vector<std::vector<Vec3i> > faces_;
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;
	TGAImage diffusemap_;
public:
	Model(std::string filename);
	~Model();
	int vertsSize();
	int facesSize();
	Vec3f Getvert(int i);
	Vec2i uv(int iface, int nvert);
	std::vector<int> Getface(int idx);
};