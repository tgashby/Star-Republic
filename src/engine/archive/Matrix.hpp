#pragma once
#include "Vector.hpp"

#define SWAP_ROWS(a, b) {float *tmp = a; a = b; b = tmp;}
#define MAT(m, r, c) m[c*4+r]

template <typename T>
struct Matrix2 {
    Matrix2()
    {
        x.x = 1; x.y = 0;
        y.x = 0; y.y = 1;
    }
    Matrix2(const T* m)
    {
        x.x = m[0]; x.y = m[1];
        y.x = m[2]; y.y = m[3];
    }
    const T* Pointer() const
    {
        return &x.x;
    }
    vec2 x;
    vec2 y;
};

template <typename T>
struct Matrix3 {
    Matrix3()
    {
        x.x = 1; x.y = 0; x.z = 0;
        y.x = 0; y.y = 1; y.z = 0;
        z.x = 0; z.y = 0; z.z = 1;
    }
    Matrix3(const T* m)
    {
        x.x = m[0]; x.y = m[1]; x.z = m[2];
        y.x = m[3]; y.y = m[4]; y.z = m[5];
        z.x = m[6]; z.y = m[7]; z.z = m[8];
    }
    Matrix3 Transposed() const
    {
        Matrix3 m;
        m.x.x = x.x; m.x.y = y.x; m.x.z = z.x;
        m.y.x = x.y; m.y.y = y.y; m.y.z = z.y;
        m.z.x = x.z; m.z.y = y.z; m.z.z = z.z;
        return m;
    }
    const T* Pointer() const
    {
        return &x.x;
    }
    vec3 x;
    vec3 y;
    vec3 z;
};

template <typename T>
struct Matrix4 {
    Matrix4()
    {
        x.x = 1; x.y = 0; x.z = 0; x.w = 0;
        y.x = 0; y.y = 1; y.z = 0; y.w = 0;
        z.x = 0; z.y = 0; z.z = 1; z.w = 0;
        w.x = 0; w.y = 0; w.z = 0; w.w = 1;
    }
    Matrix4(const Matrix3<T>& m)
    {
        x.x = m.x.x; x.y = m.x.y; x.z = m.x.z; x.w = 0;
        y.x = m.y.x; y.y = m.y.y; y.z = m.y.z; y.w = 0;
        z.x = m.z.x; z.y = m.z.y; z.z = m.z.z; z.w = 0;
        w.x = 0; w.y = 0; w.z = 0; w.w = 1;
    }
    Matrix4(const T* m)
    {
        x.x = m[0];  x.y = m[1];  x.z = m[2];  x.w = m[3];
        y.x = m[4];  y.y = m[5];  y.z = m[6];  y.w = m[7];
        z.x = m[8];  z.y = m[9];  z.z = m[10]; z.w = m[11];
        w.x = m[12]; w.y = m[13]; w.z = m[14]; w.w = m[15];
    }
    Matrix4 operator * (const Matrix4& b) const
    {
        Matrix4 m;
        m.x.x = x.x * b.x.x + x.y * b.y.x + x.z * b.z.x + x.w * b.w.x;
        m.x.y = x.x * b.x.y + x.y * b.y.y + x.z * b.z.y + x.w * b.w.y;
        m.x.z = x.x * b.x.z + x.y * b.y.z + x.z * b.z.z + x.w * b.w.z;
        m.x.w = x.x * b.x.w + x.y * b.y.w + x.z * b.z.w + x.w * b.w.w;
        m.y.x = y.x * b.x.x + y.y * b.y.x + y.z * b.z.x + y.w * b.w.x;
        m.y.y = y.x * b.x.y + y.y * b.y.y + y.z * b.z.y + y.w * b.w.y;
        m.y.z = y.x * b.x.z + y.y * b.y.z + y.z * b.z.z + y.w * b.w.z;
        m.y.w = y.x * b.x.w + y.y * b.y.w + y.z * b.z.w + y.w * b.w.w;
        m.z.x = z.x * b.x.x + z.y * b.y.x + z.z * b.z.x + z.w * b.w.x;
        m.z.y = z.x * b.x.y + z.y * b.y.y + z.z * b.z.y + z.w * b.w.y;
        m.z.z = z.x * b.x.z + z.y * b.y.z + z.z * b.z.z + z.w * b.w.z;
        m.z.w = z.x * b.x.w + z.y * b.y.w + z.z * b.z.w + z.w * b.w.w;
        m.w.x = w.x * b.x.x + w.y * b.y.x + w.z * b.z.x + w.w * b.w.x;
        m.w.y = w.x * b.x.y + w.y * b.y.y + w.z * b.z.y + w.w * b.w.y;
        m.w.z = w.x * b.x.z + w.y * b.y.z + w.z * b.z.z + w.w * b.w.z;
        m.w.w = w.x * b.x.w + w.y * b.y.w + w.z * b.z.w + w.w * b.w.w;
        return m;
    }
    Matrix4& operator *= (const Matrix4& b)
    {
        Matrix4 m = *this * b;
        return (*this = m);
    }
    bool operator == (const Matrix4& b) const
    {
        bool inx = x.x == b.x.x && x.y == b.x.y && x.z == b.x.z && x.w == b.x.w;
        bool iny = y.x == b.y.x && y.y == b.y.y && y.z == b.y.z && y.w == b.y.w;
        bool inz = z.x == b.z.x && z.y == b.z.y && z.z == b.z.z && z.w == b.z.w;
        bool inw = w.x == b.w.x && w.y == b.w.y && w.z == b.w.z && w.w == b.w.w;
        return inx && iny && inz && inw;
    }
    Matrix4 Transposed() const
    {
        Matrix4 m;
        m.x.x = x.x; m.x.y = y.x; m.x.z = z.x; m.x.w = w.x;
        m.y.x = x.y; m.y.y = y.y; m.y.z = z.y; m.y.w = w.y;
        m.z.x = x.z; m.z.y = y.z; m.z.z = z.z; m.z.w = w.z;
        m.w.x = x.w; m.w.y = y.w; m.w.z = z.w; m.w.w = w.w;
        return m;
    }
    Matrix4 Inverse()  const
    {
        float tmp[4][8];
        float m0, m1, m2, m3, s;
        float *r0, *r1, *r2, *r3;
        Matrix4 m;
        
        r0 = tmp[0];
        r1 = tmp[1];
        r2 = tmp[2];
        r3 = tmp[3];
        
        r0[0] = x.x;
        r0[1] = x.y;
        r0[2] = x.z;
        r0[3] = x.w;
        r0[4] = 1.0;
        r0[5] = r0[6] = r0[7] = 0.0;
        r1[0] = y.x;
        r1[1] = y.y;
        r1[2] = y.z;
        r1[3] = y.w;
        r1[5] = 1.0;
        r1[4] = r1[6] = r1[7] = 0.0;
        r2[0] = z.x;
        r2[1] = z.y;
        r2[2] = z.z;
        r2[3] = z.w;
        r2[6] = 1.0;
        r2[4] = r2[5] = r2[7] = 0.0;
        r3[0] = w.x;
        r3[1] = w.y;
        r3[2] = w.z;
        r3[3] = w.w;
        r3[7] = 1.0;
        r3[4] = r3[5] = r3[6] = 0.0;
        
        //Chose first pivit
        if (fabsf(r3[0]) > fabsf(r2[0]))
            SWAP_ROWS(r3, r2);
        if (fabsf(r2[0]) > fabsf(r1[0]))
            SWAP_ROWS(r2, r1);
        if (fabsf(r1[0]) > fabsf(r0[0]))
            SWAP_ROWS(r1, r0);
        if (r0[0] == 0.0) {
            return m;
        }
        //Elimate first varible
        m1 = r1[0] / r0[0];
        m2 = r2[0] / r0[0];
        m3 = r3[0] / r0[0];
        s = r0[1];
        r1[1] -= m1 * s;
        r2[1] -= m2 * s;
        r3[1] -= m3 * s;
        s = r0[2];
        r1[2] -= m1 * s;
        r2[2] -= m2 * s;
        r3[2] -= m3 * s;
        s = r0[3];
        r1[3] -= m1 * s;
        r2[3] -= m2 * s;
        r3[3] -= m3 * s;
        s = r0[4];
        if (s != 0.0) {
            r1[4] -= m1 * s;
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r0[5];
        if (s != 0.0) {
            r1[5] -= m1 * s;
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r0[6];
        if (s != 0.0) {
            r1[6] -= m1 * s;
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r0[7];
        if (s != 0.0) {
            r1[7] -= m1 * s;
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }
        //Next Pivit
        if (fabsf(r3[1]) > fabsf(r2[1]))
            SWAP_ROWS(r3, r2);
        if (fabsf(r2[1]) > fabsf(r1[1]))
            SWAP_ROWS(r2, r1);
        if (r1[1] == 0.0)
            return m;
        //Eliminate the next varible
        m2 = r2[1] / r1[1];
        m3 = r3[1] / r1[1];
        r2[2] -= m2 * r1[2];
        r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3];
        r3[3] -= m3 * r1[3];
        s = r1[4];
        if (0.0 != s) {
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r1[5];
        if (0.0 != s) {
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r1[6];
        if (0.0 != s) {
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r1[7];
        if (0.0 != s) {
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }
        //Chose the last pivit
        if (fabsf(r3[2]) > fabsf(r2[2]))
            SWAP_ROWS(r3, r2);
        if (r2[2] == 0.0)
            return m;
        //Eliminate the thrid varible
        m3 = r3[2] / r2[2];
        r3[3] -= m3 * r2[3];
        r3[4] -= m3 * r2[4];
        r3[5] -= m3 * r2[5];
        r3[6] -= m3 * r2[6];
        r3[7] -= m3 * r2[7];
        //Check one last time
        if (r3[3] == 0.0)
            return m;
        //Back substitute row 3
        s = 1.0 / r3[3];
        r3[4] *= s;
        r3[5] *= s;
        r3[6] *= s;
        r3[7] *= s;
        //Back substitue row 2
        m2 = r2[3];
        s = 1.0 / r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2);
        r2[5] = s * (r2[5] - r3[5] * m2);
        r2[6] = s * (r2[6] - r3[6] * m2);
        r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1;
        r1[5] -= r3[5] * m1;
        r1[6] -= r3[6] * m1;
        r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0;
        r0[5] -= r3[5] * m0;
        r0[6] -= r3[6] * m0;
        r0[7] -= r3[7] * m0;
        //Back substitue row 1
        m1 = r1[2];
        s = 1.0 / r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1);
        r1[5] = s * (r1[5] - r2[5] * m1);
        r1[6] = s * (r1[6] - r2[6] * m1);
        r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0;
        r0[5] -= r2[5] * m0;
        r0[6] -= r2[6] * m0;
        r0[7] -= r2[7] * m0;
        //Back substitue row 0
        m0 = r0[1];
        s = 1.0 / r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0);
        r0[5] = s * (r0[5] - r1[5] * m0);
        r0[6] = s * (r0[6] - r1[6] * m0);
        r0[7] = s * (r0[7] - r1[7] * m0);
        //Assign values to the return matrix
        m.x.x = r0[4];
        m.x.y = r0[5];
        m.x.z = r0[6];
        m.x.w = r0[7];
        m.y.x = r1[4];
        m.y.y = r1[5];
        m.y.z = r1[6];
        m.y.w = r1[7];
        m.z.x = r2[4];
        m.z.y = r2[5];
        m.z.z = r2[6];
        m.z.w = r2[7];
        m.w.x = r3[4];
        m.w.y = r3[5];
        m.w.z = r3[6];
        m.w.w = r3[7];
        return m;
    }
    Matrix2<T> ToMat2() const
    {
        Matrix2<T> m;
        m.x.x = x.x; m.y.x = y.x;
        m.x.y = x.y; m.y.y = y.y;
        return m;
    }
    Matrix3<T> ToMat3() const
    {
        Matrix3<T> m;
        m.x.x = x.x; m.y.x = y.x; m.z.x = z.x;
        m.x.y = x.y; m.y.y = y.y; m.z.y = z.y;
        m.x.z = x.z; m.y.z = y.z; m.z.z = z.z;
        return m;
    }
    const T* Pointer() const
    {
        return &x.x;
    }
    /*
    vec3 TranslatePoint(vec3 pt) const
    {
        vec3 newpt;
        newpt.x = pt.x * x.x + pt.y * x.y + pt.z * x.z + x.w;
        newpt.y = pt.x * y.x + pt.y * y.y + pt.z * y.z + y.w;
        newpt.z = pt.x * z.x + pt.y * z.y + pt.z * z.z + z.w;
        return newpt;
    }*/
    vec4 TranslatePoint(vec4 pt) const
    {
        vec4 newpt;
        newpt.x = pt.x * x.x + pt.y * y.x + pt.z * z.x + pt.w * w.x;
        newpt.y = pt.x * x.y + pt.y * y.y + pt.z * z.y + pt.w * w.y;
        newpt.z = pt.x * x.z + pt.y * y.z + pt.z * z.z + pt.w * w.z;
        newpt.w = pt.x * x.w + pt.y * y.w + pt.z * z.w + pt.w * w.w;
        return newpt;
    }
    static Matrix4<T> Identity()
    {
        return Matrix4();
    }
    static Matrix4<T> Translate(T x, T y, T z)
    {
        Matrix4 m;
        m.x.x = 1; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = 1; m.y.z = 0; m.y.w = 0;
        m.z.x = 0; m.z.y = 0; m.z.z = 1; m.z.w = 0;
        m.w.x = x; m.w.y = y; m.w.z = z; m.w.w = 1;
        return m;
    }
    static Matrix4<T> Scale(T s)
    {
        Matrix4 m;
        m.x.x = s; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = s; m.y.z = 0; m.y.w = 0;
        m.z.x = 0; m.z.y = 0; m.z.z = s; m.z.w = 0;
        m.w.x = 0; m.w.y = 0; m.w.z = 0; m.w.w = 1;
        return m;
    }
    static Matrix4<T> Scale(T x, T y, T z)
    {
        Matrix4 m;
        m.x.x = x; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = y; m.y.z = 0; m.y.w = 0;
        m.z.x = 0; m.z.y = 0; m.z.z = z; m.z.w = 0;
        m.w.x = 0; m.w.y = 0; m.w.z = 0; m.w.w = 1;
        return m;
    }
    static Matrix4<T> Rotate(T degrees)
    {
        T radians = degrees * 3.14159f / 180.0f;
        T s = std::sin(radians);
        T c = std::cos(radians);
        
        Matrix4 m = Identity();
        m.x.x =  c; m.x.y = s;
        m.y.x = -s; m.y.y = c;
        return m;
    }
    static Matrix4<T> Rotate(T degrees, const vec3& axis)
    {
        T radians = degrees * 3.14159f / 180.0f;
        T s = std::sin(radians);
        T c = std::cos(radians);
        
        Matrix4 m = Identity();
        m.x.x = c + (1 - c) * axis.x * axis.x;
        m.x.y = (1 - c) * axis.x * axis.y - axis.z * s;
        m.x.z = (1 - c) * axis.x * axis.z + axis.y * s;
        m.y.x = (1 - c) * axis.x * axis.y + axis.z * s;
        m.y.y = c + (1 - c) * axis.y * axis.y;
        m.y.z = (1 - c) * axis.y * axis.z - axis.x * s;
        m.z.x = (1 - c) * axis.x * axis.z - axis.y * s;
        m.z.y = (1 - c) * axis.y * axis.z + axis.x * s;
        m.z.z = c + (1 - c) * axis.z * axis.z;
        return m;
    }
    static Matrix4<T> Frustum(T left, T right, T bottom, T top, T near, T far)
    {
        T a = 2 * near / (right - left);
        T b = 2 * near / (top - bottom);
        T c = (right + left) / (right - left);
        T d = (top + bottom) / (top - bottom);
        T e = - (far + near) / (far - near);
        T f = -2 * far * near / (far - near);
        Matrix4 m;
        m.x.x = a; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = b; m.y.z = 0; m.y.w = 0;
        m.z.x = c; m.z.y = d; m.z.z = e; m.z.w = -1;
        m.w.x = 0; m.w.y = 0; m.w.z = f; m.w.w = 1;
        return m;
    }
    static Matrix4<T> AntiFrustum(T left, T right, T bottom, T top, T dist)
    {
        T x = dist + 1;
        T a = (left - right) / (2 * dist);
        T b = (bottom - top) / (2 * dist);
        T c = -(left + right) / (2 * x);
        T d = -(bottom + top) / (2 * x);
        T e = -dist / x;
        Matrix4 m;
        m.x.x = a; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = b; m.y.z = 0; m.y.w = 0;
        m.z.x = 0; m.z.y = 0; m.z.z = 0; m.z.w = 0;
        m.w.x = c; m.w.y = d; m.w.z = e; m.w.w = 0;
        return m;
    }
    static Matrix4<T> Parallel(T left, T right, T bottom, T top, T near, T far)
    {
        T a = 2 / (right - left);
        T b = 2 / (top - bottom);
        T d = - ((right + left)/(right - left));
        T e = - ((top + bottom)/(top - bottom));
        Matrix4 m;
        m.x.x = a; m.x.y = 0; m.x.z = 0; m.x.w = d;
        m.y.x = 0; m.y.y = b; m.y.z = 0; m.y.w = e;
        m.z.x = 0; m.z.y = 0; m.z.z = 0; m.z.w = 0;
        m.w.x = 0; m.w.y = 0; m.w.z = 0; m.w.w = 1;
        return m;
    }
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
};

typedef Matrix2<float> mat2;
typedef Matrix3<float> mat3;
typedef Matrix4<float> mat4;
