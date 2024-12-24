#pragma once
#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model { // ������
	std::vector<Vec3f> verts_; // �������
	std::vector<int> faces; // �����
	std::vector<std::vector<Vec3i> > faces_; // ����� 2
public:
	Model(std::string filename); // �����������
	~Model();
	int vertsSize(); // ��������� �������� ��������
	int facesSize();
	Vec3f Getvert(int i); // ��������� ��������
	std::vector<int> Getface(int idx);
};