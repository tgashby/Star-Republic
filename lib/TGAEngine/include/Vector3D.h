//
//  Point3D.h
//  CSC471 Graphics
//
//  Created by Taggart Ashby on 10/28/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "../include/ProjIncludes.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
//#include "GLSL_helper.h"
#include <math.h>

#ifndef CSC471_Graphics_Point3D_h
#define CSC471_Graphics_Point3D_h

class Vector3D 
{
public:
    Vector3D();
    Vector3D(GLfloat xVal, GLfloat yVal, GLfloat zVal);
    
    static void normalize(Vector3D&);
    
    Vector3D crossProd(Vector3D);
    GLfloat dotProd(Vector3D);
    
    GLfloat& operator[](int i);
    Vector3D& operator=(const Vector3D&);
    Vector3D operator*(const Vector3D&);
    Vector3D operator*(int);
    Vector3D operator*(GLfloat);
    Vector3D operator-(const Vector3D&);
    Vector3D operator+(const Vector3D&);
    operator GLfloat*();
    
    void print();
    
private:
    void setX(GLfloat);
    void setY(GLfloat);
    void setZ(GLfloat);
    void updateArr();
    
    GLfloat glArr[3];
    GLfloat x, y, z;
};

#endif
