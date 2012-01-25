//
//  BoundingSphere.cpp
//  476-Game
//
//  Created by Chad Brantley on 1/22/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <cmath>
#include <assert.h>
#include <iostream>
#include "BoundingSphere.h"
#include "BoundingCylinder.h"
#include "Coordinate.h"

BoundingSphere::BoundingSphere(float radius)
{
   this->radius = radius;
}

float BoundingSphere::getRadius()
{
   return radius;
}

bool BoundingSphere::collidesWith(BoundingObject &other, 
				  Coordinate &objLoc, Coordinate &otherLoc) 
{
  float otherRadius, changeX, changeY, changeZ, ourHeight, otherHeight;
  BoundingObject* yada = &other;

  if (typeid(other) == typeid(BoundingSphere))
    {
      otherRadius = (*(dynamic_cast<BoundingSphere*>(yada))).getRadius();
      changeX = objLoc.getX() - otherLoc.getX();
      changeY = objLoc.getY() - otherLoc.getY();
      changeZ = objLoc.getZ() - otherLoc.getZ();

      if (radius + otherRadius >
	  sqrt(changeX * changeX + changeY * changeY + changeZ * changeZ)) 
	{
	  return true;
	}
      else 
	{
	  return false;
	}

    }
  if (typeid(other) == typeid(BoundingCylinder))
    {
      otherRadius = (*(dynamic_cast<BoundingCylinder*>(yada))).getRadius();
      otherHeight = (*(dynamic_cast<BoundingCylinder*>(yada))).getHeight();
      ourHeight = radius;
      changeY = std::abs(objLoc.getY() - otherLoc.getY());
      
      if (changeY < (.5 * ourHeight) + (.5 * otherHeight)) {
	changeX = objLoc.getX() - otherLoc.getX();
	changeZ = objLoc.getZ() - otherLoc.getZ();

	if (radius + otherRadius > 
	    sqrt(changeX * changeX + changeZ * changeZ)) 
	  {
	    return true;
	  }
	else {
	  return false;
	}
      }
      else {
	return false;
      }
    }
}
