//Collection of the bounding objects required to define a GameObject
//Initial bounding structures should only be one object

BoundingStructure::BoundingStructure(std::vector<BoundingObject> boundingObjs) {
  this->boundingObjs = boundingObjs;
}

void BoundingStructure::add(BoundingObjection &obj) {
  //DOESN'T WORK YET, WHO CARES
}

BoundingObject BoundingStructure::get(int index) {
  return boundingObjs.at(index);
}
		       
int BoundingStructure::getSize() {
  return boungingObjs.size();
}

bool BoundingStructure::checkCollisions(BoundingStructure otherStruct, 
				       Coordinate objLoc, 
				       Coordinate otherLoc) {
  //ADD A CHECK FOR OVERARCING RADIUS FOR EFICIENCY
  if (true) {
    for (int i = 0; i < boundingObjs.size(); i++) {
      for (int j = 0; j < otherStruct.getSize(); j++) {
	if (boundingObjs.at(i).collidesWih(otherStruct.get(j), objLoc, 
					   otherLoc)) {
	  return true;
	}
      }
    }
  }
}
