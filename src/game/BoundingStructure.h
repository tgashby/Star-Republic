
#pragma once

#include "BoundingObject.h"

class BoundingStructure {
 public:
  BoundingStructure(std::vector<BoundingObject> boundingObjs);
  void add(BoundingObject &obj);
  int getSize();
  bool checkCollisions(BoundingStructure otherStruct,
		       Coordinate objLoc,
		       Coordinate otherLoc);
 private:
  std::vector<BoundingObject> boundingObjs;
};
