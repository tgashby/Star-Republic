//Collection of the bounding objects required to define a GameObject
//Initial bounding structures should only be one object

#include "BoundingStructure.h"

BoundingStructure::BoundingStructure(std::vector<BoundingObject *> 
				     *boundingObjs) {
  this->boundingObjs = boundingObjs;
}

BoundingObject* BoundingStructure::get(int index) {
  return (*boundingObjs).at(index);
}
		       
int BoundingStructure::getSize() {
  return (*boundingObjs).size();
}

bool BoundingStructure::checkCollisions(BoundingStructure &otherStruct, 
				       vec3 &objLoc, 
				       vec3 &otherLoc) {
  BoundingObject *obj;
  BoundingObject *other;
  
  //ADD A CHECK FOR OVERARCING RADIUS FOR EFICIENCY
  if (true) {
    for (int i = 0; i < (*boundingObjs).size(); i++) {
      obj = ((*boundingObjs).at(i));
      for (int j = 0; j < otherStruct.getSize(); j++) {
	other = (otherStruct.get(j));
	if ((*obj).collidesWith((*other), objLoc, otherLoc)) {
	  return true;
	}
      }
    }
  }
}

