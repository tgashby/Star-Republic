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

class GameObject : Drawable {
   public:
      GameObject(Coordinate startPoint, 
                 Coordinate startVelocity,
                 Angle startHeading, 
                 BoundingStructure boundingStructure
                 );
      GameObject(Coordiante startPoint, 
                 Angle startHeading, 

                 BoundingStructure boundingStructure
      //We might also need copy constructors and stuff
      void tic(int dt);
      bool setVelocity(const Coordinate velocity);
      bool setAngularVelocity(const Angle angularVelocity); 
      virtual void doCollision(GameObject & other) = 0;
      virtual Model getModel() = 0;
      const BoundingStructure getBoundingStructure() = 0;
      const Coordiante getLocation(); 
   private:
      BoundingStructure m_boundingStructure;
      Coordinate m_location;
      Coordinate m_velocity; 
      Angle m_direction;
      Angle m_angularVelocity;
};

#endif
