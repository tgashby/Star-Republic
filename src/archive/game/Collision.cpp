
#include "Collision.h"

static void checkCollisions(GameObject[] objs, int size) {
  for (int indexI = 0; indexI < size - 1; indexI++) 
    {
      for (int indexJ = indexI; indexJ < size; indexJ++)
	{
	  if (collisionCheck(objs[indexI], objs[indexJ]))
	    {
	      objs[indexI].collidesWith(objs[indexJ]);
	      objs[indexJ].collidesWith(objs[indexI]);
	    }
	}
    }
}

static bool collisionCheck(GameObject &obj, GameObject &other) {
  obj.getBoundingStructure().checkCollision(other.getBoundingStructure(), 
				    obj.getLocation(), 
					    other.getLocation());
}

int main() 
{
  
  return 0;
}
