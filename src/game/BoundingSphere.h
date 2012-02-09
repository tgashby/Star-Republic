//
//  BoundingSphere.h
//  476 Game
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Bounding_Sphere_h
#define StarRepub_Bounding_Sphere_h

#include <cmath>
#include <assert.h>
#include <iostream>
#include "BoundingSphere.h"
#include "Interfaces.h"
#include <typeinfo>

//May be held in a BoundingStructure later
//May extend BoundingObject later
class BoundingSphere {
 public:
   BoundingSphere(float radius);
   float getRadius();
   const bool collidesWith(const BoundingSphere* other, const vec3 thisPos, 
				     const vec3 otherPos) const;

   //IF WE EXTEND BOUNDING OBJECT, WILL INCLUDE POSITIONS,
   //  SO WE CAN ROTATE THEM PIECE BY PIECE

   //virtual vec3 getPosition();
   //virtual setPosition();

 private:
   float m_radius;
   //Probably would be relative to the given position inside BoundingStructure
   //vec3 m_position;
};

#endif
