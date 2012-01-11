//
//  Vector3D.cpp
//  CSC471 Graphics
//
//  Created by Taggart Ashby on 10/29/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Vector3D.h"

Vector3D::Vector3D()
: x(0), y(0), z(0)
{
    updateArr();
}

Vector3D::Vector3D(GLfloat xVal, GLfloat yVal, GLfloat zVal)
: x(xVal), y(yVal), z(zVal)
{
    updateArr();
}

void Vector3D::normalize(Vector3D& v) {
    // normalize v[] and return the result in v[]
    // from OpenGL Programming Guide, p. 58
    GLfloat d = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (d == 0.0) {
        printf("zero length vector");
        return;
    }
    v[0] = v[0]/d; 
    v[1] = v[1]/d; 
    v[2] = v[2]/d;
}

Vector3D Vector3D::crossProd(Vector3D v) {
    // cross v1[] and v2[] and return the result in out[]
    // from OpenGL Programming Guide, p. 58
    Vector3D toRet;
    
    toRet.x = y*v[2] - z*v[1];
    toRet.y = z*v[0] - x*v[2];
    toRet.z = x*v[1] - y*v[0];
    normalize(toRet);
    return toRet;
}

GLfloat Vector3D::dotProd(Vector3D v)
{
    Vector3D toRet;
    
    toRet.x = x * v.x;
    toRet.y = y * v.y;
    toRet.z = z * v.z;
    
    return toRet.x + toRet.y + toRet.z;
}

GLfloat& Vector3D::operator[](int i)
{
    switch (i) {
        case 0:
            return x;
            break;
            
        case 1:
            return y;
            break;
            
        case 2:
            return z;
            
        default:
            return x;
            break;
    }   
}

Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    
    updateArr();
    
    return *this;
}

Vector3D Vector3D::operator*(const Vector3D& rhs)
{
    Vector3D v(x, y, z);
    v.x *= rhs.x;
    v.y *= rhs.y;
    v.z *= rhs.z;
    
    return v;
}

Vector3D Vector3D::operator*(int scale)
{
    Vector3D v(x, y, z);
    v.x *= scale;
    v.y *= scale;
    v.z *= scale;
    
    return v;
}

Vector3D Vector3D::operator*(GLfloat scale)
{
    Vector3D v(x, y, z);
    v.x *= scale;
    v.y *= scale;
    v.z *= scale;
    
    return v;
}

Vector3D Vector3D::operator-(const Vector3D& rhs)
{
    Vector3D v(x, y, z);
    v.x -= rhs.x;
    v.y -= rhs.y;
    v.z -= rhs.z;
    
    return v;
}

Vector3D Vector3D::operator+(const Vector3D& rhs)
{
    Vector3D v(x, y, z);
    v.x += rhs.x;
    v.y += rhs.y;
    v.z += rhs.z;
    
    return v;
}

Vector3D::operator GLfloat*()
{
    return glArr;
}

void Vector3D::print() {
    std::cout << "(";
    std::cout << x << ", " << y << ", " << z;
    std::cout << ")" << std::endl;
}

void Vector3D::setX(GLfloat val)
{
    x = val;
    updateArr();
}

void Vector3D::setY(GLfloat val)
{
    y = val;
    updateArr();
}

void Vector3D::setZ(GLfloat val)
{
    z = val;
    updateArr();
}

void Vector3D::updateArr()
{
    glArr[0] = x;
    glArr[1] = y;
    glArr[2] = z;
}