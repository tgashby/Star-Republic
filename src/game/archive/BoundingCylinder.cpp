//
//  BoundingCylinder.cpp
//  476-Game
//
//  Created by Chad Brantley on 1/22/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <cmath>
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

bool BoundingCylinder::collidesWith(BoundingObject &other, Coordinate &objLoc, 
				    Coordinate &otherLoc) 
{
  float otherRadius, changeX, changeY, changeZ, otherHeight;
  BoundingObject* yada = &other;

  if (typeid(other) == typeid(BoundingSphere))
    {
      otherRadius = (*(dynamic_cast<BoundingSphere*>(yada))).getRadius();
      otherHeight = otherRadius;
      changeY = std::abs(objLoc.getY() - otherLoc.getY());
      
      if (changeY < (.5 * height) + (.5 * otherHeight))
	{
	  changeX = objLoc.getX() - otherLoc.getX();
	  changeZ = objLoc.getZ() - otherLoc.getZ();
	  
	  if(radius + otherRadius >
	     std::sqrt(changeX * changeX + changeZ * changeZ)) 
	    {
	      return true;
	    }
	  else 
	    {
	      return false;
	    }
	}
      else 
	{
	  return false;
	}
    }
  if (typeid(other) == typeid(BoundingCylinder))
    {
      otherRadius = (*(dynamic_cast<BoundingCylinder*> (yada))).getRadius();
      otherHeight = (*(dynamic_cast<BoundingCylinder*> (yada))).getHeight();
      changeY = std::abs(objLoc.getY() - otherLoc.getY());

      if (changeY < (.5 * height) + (.5 * otherHeight))
	{
	  changeX = objLoc.getX() - otherLoc.getX();
	  changeZ = objLoc.getZ() - otherLoc.getZ();
	  
	  if(radius + otherRadius >
	     std::sqrt(changeX * changeX + changeZ * changeZ)) 
	    {
	      return true;
	    }
	  else 
	    {
	      return false;
	    }
	}
      else 
	{
	  return false;
	}
    }
}
