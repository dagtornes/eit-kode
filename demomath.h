#pragma once


#include <cmath>
#include <vector>

static const float PI = 3.1415926535897932384626433832f;

struct Vector3
{
    Vector3() : x(), y(), z(){}
    Vector3(float a, float b, float c) : x(a), y(b), z(c){}
    float x,y,z;
};

class Matrix4
{
public:
    Matrix4(){  zero(); }
    void zero(){ std::fill(data, data+16, 0.0f); }
    void identity(){ zero(); data[0] = data[5] = data[10] = data[15] = 1.0f; }
    float& operator[](size_t index){ return data[index]; }
    const float* c_ptr() const { return data; }
private:
    float data[16];
};

Matrix4 CreatePerspectiveProjection(float fov, float aspect, float near, float far)
{
    Matrix4 mat;
    float angle = (fov / 180.0f) * PI;
    float f = 1.0f / std::tan( angle * 0.5f );
 
    mat[0] = f / aspect;
    mat[5] = f;
    mat[10] = (far + near) / (near - far);
    mat[11] = -1.0f;
    mat[14] = (2.0f * far*near) / (near - far);
 
    return mat;
}
