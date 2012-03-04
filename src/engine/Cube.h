//
//  Cube.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/18/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_Cube_h
#define StarRepublic_Cube_h

#include "Plane.h"

class Cube 
{
public:
   /**
    *
    */
   Cube(Plane lPlane, Plane rPlane, Plane uPlane, Plane dPlane,
        Plane nPlane, Plane fPlane);
   
   /**
    *
    */
   Cube(Matrix4<float> mat);
   
   /**
    *
    */
   bool checkPoint(Vector3<float> point);
   
private:
   Plane leftPlane;
   Plane rightPlane;
   Plane upPlane;
   Plane downPlane;
   Plane nearPlane;
   Plane farPlane;
};

#endif
