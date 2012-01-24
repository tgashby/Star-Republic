
#include "GameObject.h"

class Collision {
 public:
  static void checkCollisions(GameObject[] objs, int size);

 private:
  static void collisionCheck(GameObject &one, GameObject &other);
}
