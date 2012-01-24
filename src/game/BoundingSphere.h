//
//  BoundingSphere.h
//  476 Game
//
//  Created by Chad Brantley on 1/22/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <typeinfo>
#include "BoundingObject.h"

class BoundingSphere: public BoundingObject {

public:
   BoundingSphere(float radius);
   float getRadius();
   virtual bool collidesWith(BoundingObject &other);
   
private:
   float radius;

};
