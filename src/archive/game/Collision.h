
#pragma once

#include "GameObject.h"

/** The Collision class is the main utility to check for collision
 *
 * @author Chad Brantley
 * @version 1.0
 */
class Collision {
 public:

  /** Compares all given objects to each other, and invoke the effects of the
   *  collision if not given
   *
   * @param objs The list of objects to be checked for collisions
   * @param size The size of the list of objects
   */
  static void checkCollisions(GameObject[] objs, int size);

 private:
  /** Checks whether two objects collide
   *
   * @param obj The first object to check for a collision
   * @param other The other object to check for a collision
   */
  static bool collisionCheck(GameObject &obj, GameObject &other);
}
