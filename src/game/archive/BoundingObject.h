/* 
 * BoundingObject - Define a hitbox for an object or portion of one
 */

#pragma once
#include <typeinfo>
#include "Coordinate.h"

class BoundingObject {
 public:
  virtual bool collidesWith(BoundingObject &other, 
			    Coordinate &thisLoc, Coordinate &otherLoc) = 0;
};
