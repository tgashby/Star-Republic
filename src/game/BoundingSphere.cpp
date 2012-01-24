//
//  BoundingSphere.cpp
//  476-Game
//
//  Created by Chad Brantley on 1/22/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BoundingSphere.h"
#include "BoundingCylinder.h"

BoundingSphere::BoundingSphere(float radius)
{
   this->radius = radius;
}

float BoundingSphere::getRadius()
{
   return radius;
}

bool BoundingSphere::collidesWith(BoundingObject &other) 
{
  int otherRadius;
  if (typeid(other) == typeid(BoundingSphere))
    {
      std::cout << "Comparing sphere to a sphere";
      otherRadius = (BoundingSphere) other.getRadius();
    }
  if (typeid(other) == typeid(BoundingCylinder))
    {
      std::cout << "Comparing sphere to a cylinder";
    }
}
