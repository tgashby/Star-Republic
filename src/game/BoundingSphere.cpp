//
//  BoundingSphere.cpp
//  476 StarRepub
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(float radius) {
   m_radius = radius;
}

float BoundingSphere::getRadius() {
   return m_radius;
}

bool BoundingSphere::collidesWith(BoundingSphere* other, vec3 thisPos, 
				  vec3 otherPos) {
   float otherRadius;
   
   if(m_radius + otherRadius > (thisPos - otherPos).Length()) {
      return true;
   }
   else {
      return false;
   }
}
