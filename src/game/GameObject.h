/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Drawable.h"

class GameObject : Drawable {
   public:
      void tic(int dt);
      bool setVelocity(const Coordinate velocity);
      bool setAngularVelocity(const Angle angularVelocity); 
      virtual void doCollision(GameObject & other) = 0;
      virtual Model getModel() = 0;
      virtual BoundingObject getBoundingObject() = 0;
   private:
      Coordiante m_location;
      Coordiante m_velocity; 
      Angle m_direction;
      Angle m_angularVelocity;
};

#endif
