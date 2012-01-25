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

int main()
{
  float aRadius = 1;
  BoundingSphere *obj = new BoundingSphere(aRadius);
  BoundingSphere *other = new BoundingSphere(aRadius);
  BoundingCylinder *cylinder = new BoundingCylinder(aRadius, 2.0);
  Coordinate *loc1 = new Coordinate(0, 0, 0);
  Coordinate *loc2 = new Coordinate(0, 0, 0);
  Coordinate *loc3 = new Coordinate(0, 0.5, 0.5);
  Coordinate *loc4 = new Coordinate(.25, .25, .25);
  
  Coordinate *loc5 = new Coordinate(0, 2, 0);
  Coordinate *loc6 = new Coordinate(3, 3, 3);
  Coordinate *loc7 = new Coordinate(0, 10, 0);
  Coordinate *loc8 = new Coordinate(0, 1.5, 0);

  assert((*obj).collidesWith(*other, *loc1, *loc2));
  assert((*obj).collidesWith(*other, *loc1, *loc3));
  assert((*obj).collidesWith(*other, *loc1, *loc4));
  assert(!(*obj).collidesWith(*other, *loc1, *loc5));
  assert(!(*obj).collidesWith(*other, *loc3, *loc6));
  assert(!(*obj).collidesWith(*other, *loc4, *loc6));

  assert((*obj).collidesWith(*cylinder, *loc1, *loc2));
  assert((*cylinder).collidesWith(*cylinder, *loc1, *loc2));
  assert((*cylinder).collidesWith(*cylinder, *loc1, *loc8));
  assert(!(*cylinder).collidesWith(*cylinder, *loc6, *loc7));

  return 0;
}
