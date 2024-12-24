#include "geometry.h" // Все необходимое
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "Model.h"
#include "tgaimage.h"
#include <string>
using namespace std;

Vec3f eye(2, 0, 10); // Камера
Vec3f center(0, 0, 3); // Куда смотрит камера
Vec3f camera(0, 0, 10); // z расстояние камеры
int* zbuffer = NULL; 
const int depth = 255; // Глубина z-buffer 
const Vec3f light_dir(0, 0, -1); // Свет
int width = 800;
int height = 800;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

// Переход между матрицей и вектором и наоборот
Vec3f matrix2v(Matrix m) {
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
} 
Matrix vector2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

// Для перемещения камеры
Matrix viewportMatrix(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}
Matrix glalookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();
    Matrix res = Matrix::identity(4);
    for (int i = 0; i < 3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}

// Растеризация треугольника
void triangle(Vec3i t0, Vec3i t1, Vec3i t2, TGAImage& image, TGAColor color, int* zbuffer) {
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    int total_height = t2.y - t0.y;
    for (int y = t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t0.y) / segment_height; 
        Vec3i A = t0 + (t2 - t0) * alpha;
        Vec3i B = t0 + (t1 - t0) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            int z = A.z + (B.z - A.z) * phi;
            int idx = j + y * width;
            if (idx < 0) break;
            if (idx > width * height) break;
            if (zbuffer[idx] < z) {
                zbuffer[idx] = z;
                image.set(j, y, color);
            }
        }
    }
    for (int y = t1.y; y <= t2.y; y++) {
        int segment_height = t2.y - t1.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t1.y) / segment_height; 
        Vec3i A = t0 + (t2 - t0) * alpha;
        Vec3i B = t1 + (t2 - t1) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            int z = A.z + (B.z - A.z) * phi;
            int idx = j + y * width;
            if (idx < 0) break;
            if (idx > width*height) break;
            if (zbuffer[idx] < z) {
                zbuffer[idx] = z;
                image.set(j, y, color);
            }
        }
    }
}

int main() {
	setlocale(LC_ALL, "rus");
    cout << "EXTRA ENGINE 3000. Версия: 1.0.0" << endl;
    cout << "Введите путь до файла формата obj: ";
    string name;
    cin >> name;
    //std::string source("obj/african_head.obj");
    //std::string target("obj/african_headT.obj");
    cout << endl << "Парсим..." << endl;
    Model model(name); // Парсим нашу модельку

    string a;
    cout << "Напиши 0 для стандартных значений: ";
    cin >> a;
    if (a != "0") {
        cout << "eye(Координаты камеры)(По умолчанию (2, 0, 10)): ";
        cin >> eye.x >> eye.y >> eye.z;
        cout << endl;
        cout << "center(Куда смотрит камера)(По умолчанию (0, 0, 3)): ";
        cin >> center.x >> center.y >> center.z;
        cout << endl;
        cout << "camera(z координата камеры для перспективы)(По умолчанию 10): ";
        cin >> camera.z;
        cout << endl;
        cout << "width(Изображения)(По умолчанию 800): ";
        cin >> width;
        cout << endl;
        cout << "height(Изображения)(По умолчанию 800): ";
        cin >> height;
        cout << endl;
    }

    zbuffer = new int[width * height]; // Инициализируем z-buffer

    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    // Все матрицы преоразования(Для поворота и перемещения камеры)
    Matrix ModelView = glalookat(eye, center, Vec3f(0, 1, 0));
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort = viewportMatrix(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    Projection[3][2] = -1.f / camera.z; // Перспективная проекция

    cout << "Создание изображения..." << endl;

    TGAImage image(width, height, TGAImage::RGB);

    for (int i = 0; i < model.facesSize(); i++) { // Проходимся по полигонам
        std::vector<int> face = model.Getface(i);
        Vec3i screen_coords[3]; // Экранные координаты
        Vec3f world_coords[3]; // Мировые координаты
        for (int j = 0; j < 3; j++) { // Работаем с полигоном
            Vec3f v = model.Getvert(face[j]);
            Vec3f v2 = matrix2v(ViewPort * Projection * ModelView * vector2m(v)); // Умножаем на матрицы преобразований
            screen_coords[j] = Vec3i(int(v2.x+.5), int(v2.y+.5), int(v2.z+.5)); // Переводим в экранные координаты
            world_coords[j] = v;
        }
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]); // Нормаль
        n.normalize();
        float intensity = n * light_dir; // Считаем свет
        if (intensity > 0) {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255), zbuffer);
        } // Наконец рисуем. Ура!
    }

    cout << "Готово!" << endl; // Победа

    image.flip_vertically(); 
    image.write_tga_file("Render.tga");
    return 0;
}