//
//  BoundingCylinder.h
//  476 Game
//
//  Created by Chad Brantley on 1/22/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <typeinfo>
#include "BoundingObject.h"

class BoundingCylinder: public BoundingObject {

public:
  BoundingCylinder(float radius, float height);
   float getRadius();
   float getHeight();
   virtual bool collidesWith(BoundingObject &other);
   
private:
   float radius;
   float height;

};
