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

const bool BoundingSphere::collidesWith (const BoundingSphere* other,const vec3 thisPos, const vec3 otherPos) const{
   float otherRadius;
   
   if(m_radius + otherRadius > (thisPos - otherPos).Length()) {
      return true;
   }
   else {
      return false;
   }
}
