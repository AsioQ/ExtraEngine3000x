#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cmath>
#include <fstream>
#include <vector>

template <class t> struct Vec2 {
    t x, y;
    Vec2<t>() : x(t()), y(t()) {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
    Vec2<t>(const Vec2<t>& v) : x(t()), y(t()) { *this = v; }
    Vec2<t>& operator =(const Vec2<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }
    Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(x + V.x, y + V.y); }
    Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(x - V.x, y - V.y); }
    Vec2<t> operator *(float f)          const { return Vec2<t>(x * f, y * f); }
    t& operator[](const int i) { if (x <= 0) return x; else return y; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
    t x, y, z;
    Vec3<t>() : x(t()), y(t()), z(t()) { }
    Vec3<t>(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    template <class u> Vec3<t>(const Vec3<u>& v);
    Vec3<t>(const Vec3<t>& v) : x(t()), y(t()), z(t()) { *this = v; }
    Vec3<t>& operator =(const Vec3<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }
    Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
    Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
    Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
    t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
    t& operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
};

/*class Vec3i {
public:
    int x, y, z;
    Vec3i() : x(int()), y(int()), z(int()) { }
    Vec3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    Vec3i(const Vec3i& v) : x(int()), y(int()), z(int()) { *this = v; }
    Vec3i& operator =(const Vec3i& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }
    Vec3i operator ^(const Vec3i& v) const { return Vec3i(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3i operator +(const Vec3i& v) const { return Vec3i(x + v.x, y + v.y, z + v.z); }
    Vec3i operator -(const Vec3i& v) const { return Vec3i(x - v.x, y - v.y, z - v.z); }
    Vec3i operator *(float f) const { return Vec3i(x * f, y * f, z * f); }
    int operator *(const Vec3i& v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3i& normalize(int l = 1) { *this = (*this) * (l / norm()); return *this; }
    int& operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }
    friend std::ostream& operator<<(std::ostream& s, Vec3i& v) {
        s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return s;
    }
};

class Vec3f {
public:
    float x, y, z;
    Vec3f() : x(float()), y(float()), z(float()) { }
    Vec3f(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    Vec3f(const Vec3f& v) : x(float()), y(float()), z(float()) { *this = v; }
    Vec3f& operator =(const Vec3f& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }
    Vec3f operator ^(const Vec3f& v) const { return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3f operator +(const Vec3f& v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
    Vec3f operator -(const Vec3f& v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
    Vec3f operator *(float f)          const { return Vec3f(x * f, y * f, z * f); }
    float operator *(const Vec3f& v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3f& normalize(float l = 1) { *this = (*this) * (l / norm()); return *this; }
    float& operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }
    friend std::ostream& operator<<(std::ostream& s, Vec3f& v) {
        s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return s;
    }
};*/

//////////////////////////////////////////////////////////////////////////////////

const int DEFAULT_ALLOC = 4;

class Matrix {
    std::vector<std::vector<float> > m;
    int rows, cols;
public:
    int ncols();
    int nrows();

    static Matrix identity(int dimensions);
    Matrix operator*(const Matrix& a);
    std::vector<float>& operator[](const int i);

    Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
};

#endif //__GEOMETRY_H__