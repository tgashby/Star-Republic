//
//  Cube.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/18/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "Cube.h"

Cube::Cube(Plane lPlane, Plane rPlane, Plane uPlane, Plane dPlane,
     Plane nPlane, Plane fPlane)
{
   leftPlane = lPlane;
   rightPlane = rPlane;
   upPlane = uPlane;
   downPlane = dPlane;
   nearPlane = nPlane;
   farPlane = fPlane;
   
   Plane::Normalize(leftPlane);
   Plane::Normalize(rightPlane);
   Plane::Normalize(downPlane);
   Plane::Normalize(upPlane);
   Plane::Normalize(nearPlane);
   Plane::Normalize(farPlane);
}

Cube::Cube(Matrix4<float> mat)
{
   leftPlane.a = mat.x.w + mat.x.x;
   leftPlane.b = mat.y.w + mat.y.x;
   leftPlane.c = mat.z.w + mat.z.x;
   leftPlane.d = mat.w.w + mat.w.x;
   Plane::Normalize(leftPlane);
   
   rightPlane.a = mat.x.w - mat.x.x;
   rightPlane.b = mat.y.w - mat.y.x;
   rightPlane.c = mat.z.w - mat.z.x;
   rightPlane.d = mat.w.w - mat.w.x;
   Plane::Normalize(rightPlane);
   
   upPlane.a = mat.x.w - mat.x.y;
   upPlane.b = mat.y.w - mat.y.y;
   upPlane.c = mat.z.w - mat.z.y;
   upPlane.d = mat.w.w - mat.w.y;
   Plane::Normalize(upPlane);
   
   downPlane.a = mat.x.w + mat.x.y;
   downPlane.b = mat.y.w + mat.y.y;
   downPlane.c = mat.z.w + mat.z.y;
   downPlane.d = mat.w.w + mat.w.y;
   Plane::Normalize(downPlane);
   
   nearPlane.a = mat.x.w + mat.x.z;
   nearPlane.b = mat.y.w + mat.y.z;
   nearPlane.c = mat.z.w + mat.z.z;
   nearPlane.d = mat.w.w + mat.w.z;
   Plane::Normalize(nearPlane);
   
   farPlane.a = mat.x.w - mat.x.z;
   farPlane.b = mat.y.w - mat.y.z;
   farPlane.c = mat.z.w - mat.z.z;
   farPlane.d = mat.w.w - mat.w.z;
   Plane::Normalize(farPlane);
}

bool Cube::checkPoint(Vector3<float> point)
{
   HalfSpace spaces[6];
   int i;
   
   spaces[0] = Plane::ClassifyPoint(leftPlane, point);
   spaces[1] = Plane::ClassifyPoint(rightPlane, point);
   spaces[2] = Plane::ClassifyPoint(upPlane, point);
   spaces[3] = Plane::ClassifyPoint(downPlane, point);
   spaces[4] = Plane::ClassifyPoint(nearPlane, point);
   spaces[5] = Plane::ClassifyPoint(farPlane, point);
   
   for (i = 0; i < 6; i++) 
   {
      if (spaces[i] == OUTSIDE)
         break;
   }
   
   // If i is 6 (point is inside all planes), don't cull!
   return i == 6;
}
