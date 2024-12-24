#pragma once
#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model { // Модель
	std::vector<Vec3f> verts_; // Вершины
	std::vector<int> faces; // грани
	std::vector<std::vector<Vec3i> > faces_; // Грани 2
public:
	Model(std::string filename); // Конструктор
	~Model();
	int vertsSize(); // Получение размеров массивов
	int facesSize();
	Vec3f Getvert(int i); // Получение значений
	std::vector<int> Getface(int idx);
};