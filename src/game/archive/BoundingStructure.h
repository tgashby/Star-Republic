
#pragma once

#include "BoundingObject.h"
#include <vector>

class BoundingStructure {

 public:
  BoundingStructure(std::vector<BoundingObject *> *boundingObjs);
  int getSize();
  BoundingObject* get(int index);
  bool checkCollisions(BoundingStructure &otherStruct,
		       Coordinate &objLoc,
		       Coordinate &otherLoc);
 private:
  std::vector<BoundingObject *> *boundingObjs;
};
