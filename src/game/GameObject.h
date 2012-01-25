/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Drawable.h"
#include "Angle.h"
#include "BoundingStructure.h"
#include "Vector.hpp"

class GameObject : Drawable {
   public:
      GameObject(Vector3<float> startPoint, 
                 Vector3<float> startVelocity,
                 Angle startHeading, 
                 BoundingStructure boundingStructure
                 );
      void tic(int dt);
      bool setVelocity(const Vector3<float> velocity);
      bool setAngularVelocity(const Angle angularVelocity); 
      virtual void doCollision(GameObject & other) = 0;
      virtual Model getModel() = 0;
      const BoundingStructure getBoundingStructure();
      const Vector3<float> getLocation(); 
   private:
      BoundingStructure m_boundingStructure;
      Vector3<float> m_location;
      Vector3<float> m_velocity; 
      Angle m_direction;
      Angle m_angularVelocity;
};

#endif
