#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"

Model::Model(std::string filename) : verts_(), faces_(), norms_(), uv_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            norms_.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            int f, t, n;
            std::vector<Vec3i> f0;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                Vec3i f1;
                f1.x = --f;
                f0.push_back(f1);
                cnt++;
            }
            if (3 != cnt) {
                std::cerr << "ОШИБКА НАХУЙ!" << std::endl;
                return;
            }
            faces_.push_back(f0);
        }
    }

    std::cerr << "Вертексы# " << verts_.size() << " Фейсы# " << faces_.size() << norms_.size() << std::endl;
}

Model::~Model() {
}

Vec2i Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert][1];
    return Vec2i(uv_[idx].x * diffusemap_.get_width(), uv_[idx].y * diffusemap_.get_height());
}

int Model::vertsSize() { // Размер вектора
    return (int)verts_.size();
}

int Model::facesSize() { // Тоже самое
    return (int)faces_.size();
}

std::vector<int> Model::Getface(int idx) {
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
    return face;
}

Vec3f Model::Getvert(int i) {
    return verts_[i];
}