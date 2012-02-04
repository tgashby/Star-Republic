//
//  BoundingSphere.h
//  476 Game
//
//  Created by Chad Brantley on 1/22/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <typeinfo>
#include "BoundingObject.h"

class BoundingSphere: public BoundingObject {

public:
   BoundingSphere(float radius);
   float getRadius();
   virtual bool collidesWith(BoundingObject &other, Coordinate &thisLoc, 
			     Coordinate &otherLoc);
   
private:
   float radius;
   //COORDINATE WILL EVENTUALLY NEED TO BE ADDED TO ALLOW US TO SPECIFY WHERE IN REGARDS TO CENTER OF OBJECT

};
