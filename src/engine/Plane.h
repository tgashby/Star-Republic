//
//  Plane.h
//  Labs
//
//  Created by Taggart Ashby on 2/6/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef Labs_Plane_h
#define Labs_Plane_h

#include "Matrix.h"

enum HalfSpace 
{
   INSIDE = -1,
   ON = 0,
   OUTSIDE = 1
};

class Plane
{
public:
   static void Normalize(Plane& plane);
   static float DistanceToPoint(const Plane& plane, const Vector3<float>& point);
   static HalfSpace ClassifyPoint(const Plane& plane, const Vector3<float>& point);
   static Plane MakePlane(vec3 normal, vec3 position);
   
   float a, b, c, d;
   
private:
   
};

#endif
