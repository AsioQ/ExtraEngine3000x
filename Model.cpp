#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"

Model::Model(std::string filename) : verts_(), faces_(){
    std::ifstream in; // Открываем файл
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line); // Берем строку
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) { // Если вершина
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v); // Пушим
        }
        else if (!line.compare(0, 2, "f ")) { // Если грань
            int f, t, n;
            std::vector<Vec3i> f0;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                Vec3i f1;
                f1.x = --f;
                f0.push_back(f1); // Пушим
                cnt++;
            }
            if (3 != cnt) {
                std::cerr << "ОШИБКА НАХУЙ! Триангулируй obj!" << std::endl;
                return;
            }
            faces_.push_back(f0); // Пушим
        }
    }

    std::cerr << "Вертексы# " << verts_.size() << " Фейсы# " << faces_.size() << std::endl; // Для отладки
}

Model::~Model() {
}

int Model::vertsSize() { // Размер вектора
    return (int)verts_.size();
}

int Model::facesSize() { // Тоже самое
    return (int)faces_.size();
}

std::vector<int> Model::Getface(int idx) { // Получаем значения
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
    return face;
}

Vec3f Model::Getvert(int i) {
    return verts_[i];
}