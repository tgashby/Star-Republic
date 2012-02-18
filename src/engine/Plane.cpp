//
//  Plane.cpp
//  Labs
//
//  Created by Taggart Ashby on 2/6/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "Plane.h"

void Plane::Normalize(Plane& plane)
{
   float mag;
   
   mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);
   
   plane.a /= mag;
   plane.b /= mag;
   plane.c /= mag;
   plane.d /= mag;
}

float Plane::DistanceToPoint(const Plane &plane, const Vector3<float>& point)
{
   return plane.a*point.x + plane.b*point.y + plane.c*point.z + plane.d;
}

HalfSpace Plane::ClassifyPoint(const Plane &plane, const Vector3<float> &point)
{
   float dist;
   
   dist = Plane::DistanceToPoint(plane, point);
   
   if (dist < 0) 
   {
      return OUTSIDE;
   }
   
   if (dist > 0) 
   {
      return INSIDE;
   }
   
   return ON;
}

Plane Plane::MakePlane(vec3 normal, vec3 position)
{
   Plane toRet;
   
   toRet.a = normal.x;
   toRet.b = normal.y;
   toRet.c = normal.z;
   toRet.d = -(position.x * toRet.a + position.y * toRet.b + position.z * toRet.c);
   
   return toRet;
}
