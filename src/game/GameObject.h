/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Interfaces.h"
#include "BoundingSphere.h"

class GameObject {
public:
   // Constructors
   GameObject(vec3 startPos, vec3 startVelocity, 
              vec3 forwardVec, vec3 accelerationVec, 
              vec3 upVec,float collideRadius );
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
   const vec3 getLocation() const;
   const Vector3<float> getPosition() const;
   const Vector3<float> getForward() const;
   const Vector3<float> getUp() const;
   
   // Bounding and Collision
   virtual void doCollision(GameObject & other) = 0;
   //In the future this will be replaced with a more complex boundingStructure
   virtual bool collidesWith(const GameObject & other) const; 
   
      
   const BoundingSphere * getBoundingSphere() const;
   //const BoundingHeirarchy getBounds();
protected:
   vec3 m_position;
   vec3 m_velocity;
   vec3 m_forward;
   vec3 m_acceleration;
   vec3 m_up;
   
private:
   static const float defaultBoundingRadius = 4;
   const BoundingSphere * m_boundingSphere; 
   // BoundingHeirarchy bounds;
};

#endif
