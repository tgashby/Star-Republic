
#include "Collisions.h"

static void checkCollisions(GameObject[] objs, int size) 
{
  for (int indexI = 0; indexI < size - 1; indexI++) 
    {
      for (int indexJ = indexI; indexJ < size; indexJ++)
	{
	  collisionCheck(objs[indexI], objs[indexJ]);
	}
    }
}

static void collisionCheck(GameObject &one, GameObject &other) {
  
}
