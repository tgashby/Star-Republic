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

BoundingCylinder::BoundingCylinder(float radius, float height)
{
   this->radius = radius;
   this->height = height;
}

float BoundingCylinder::getRadius()
{
   return radius;
}

float BoundingCylinder::getHeight()
{
  return height;
}

bool BoundingCylinder::collidesWith(BoundingObject &other) 
{
  if (typeid(other) == typeid(BoundingSphere))
    {
      std::cout << "Comparing cylinder to a sphere";
    }
  if (typeid(other) == typeid(BoundingCylinder))
    {
      std::cout << "Comparing cylinder to a cylinder";
    }
}
