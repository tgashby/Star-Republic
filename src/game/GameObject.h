/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Interfaces.h"
#include "Angle.h"
#include "BoundingStructure.h"

class GameObject : IObject3d {
   public:
      GameObject(vec3 startPoint, 
                 vec3 startVelocity,
                 Angle startHeading, 
                 BoundingStructure boundingStructure
                 );
      ~GameObject();
      void tic(int dt);
      bool setVelocity(const Vector3<float> velocity);
      bool setAngularVelocity(const Angle angularVelocity); 
      //virtual void doCollision(GameObject & other) = 0;
      list<IMesh *>* getMeshes();
      const BoundingStructure getBoundingStructure();
      const vec3 getLocation(); 
   private:
      list<IMesh *> m_meshList;
      BoundingStructure m_boundingStructure;
      vec3 m_location;
      vec3 m_velocity; 
      Angle m_direction;
      Angle m_angularVelocity;
};

#endif
