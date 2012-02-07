//
//  BoundingStructure.h
//  476 Game
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Bounding_Structure_h
#define StarRepub_Bounding_Structure_h

#include <cmath>
#include <assert.h>
#include <iostream>
#include <typeinfo>
#include <vector>

#include "BoundingSphere.h"
#include "Interfaces.h"

class BoundingStructure {
public:
   BoundingStructure();
   void addBoundingObject();
   bool collidesWith(BoundingStructure* other);
   void updatePosition();
private:
   vec3 up, forward, position;
   std::vector<BoundingSphere*> objects;
};

#endif
