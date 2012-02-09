/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Interfaces.h"

class GameObject {
public:
   // Constructors
   GameObject(vec3 startPos, vec3 startVelocity, 
              vec3 forwardVec, vec3 accelerationVec, 
              vec3 upVec);
   GameObject();
   
   // Destructors
   ~GameObject();
   
   // Update function
   virtual void tic(uint64_t time);
   
   // Setters
   void setVelocity(const Vector3<float> velocity);
   void setUp(Vector3<float> upVal);
   void setPosition(Vector3<float> pos);
   void setAcceleration(Vector3<float> acc);
   void setForward(Vector3<float> forward);
   
   // Getters
   const vec3 getLocation();
   const Vector3<float> getPosition();
   const Vector3<float> getForward();
   const Vector3<float> getUp();
   
   // Bounding and Collision
   //virtual void doCollision(GameObject & other) = 0;
   //const BoundingHeirarchy getBounds();
protected:
   vec3 m_position;
   vec3 m_velocity;
   vec3 m_forward;
   vec3 m_acceleration;
   vec3 m_up;
   
   // BoundingHeirarchy bounds;
};

#endif
