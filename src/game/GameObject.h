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
      GameObject(Coordiante startPoint, 
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
      Coordiante m_location;
      Coordiante m_velocity; 
      Angle m_direction;
      Angle m_angularVelocity;
};

#endif
