/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

#ifndef StarRepub_GameObject_h
#define StarRepub_GameObject_h

#include "../engine/Interfaces.h"
#include "BoundingSphere.h"


/**
 * GameObject base class definition. All movable in-game objects are derived
 * from this class
 */
class GameObject {
public:
   /**
    * GameObject Constructor. Used for creating a new game object from scrach.
    * @param startPoint - The point where the object is created
    * @param startHeading - The direction that the model is pointed when it is
    * created.
    * @param startVelocity - The speed and direction of the object when it is 
    * created.
    * @param boundingStructure - The game object's bounding structure
    */ 
   GameObject(vec3 startPos, vec3 startVelocity, 
              vec3 forwardVec, vec3 accelerationVec, 
              vec3 upVec,float collideRadius, int health);

   GameObject(vec3 startPos, float radius);
   
   /**
    * Default constructor that sets all fields to default values
    */
   GameObject();
   
   // Destructors
   ~GameObject();
   
   /**
    * tic - Runs once per game loop to update the game object's location and
    * heading.
    * @param int dt - The amount of time it took to draw the previous frame.
    */
   virtual void tic(uint64_t time);
   
   /**
    * setVelocity - sets the speed and direction of a GameObject. 
    * @param velocity - the new velocity
    */
   void setVelocity(const Vector3<float> velocity);
   
   /**
    * setUp sets the object's up vector
    * @param upVal the new up vector
    */
   void setUp(Vector3<float> upVal);
   
   /**
    * setPosition sets the object's position in the world
    * @param pos the new position for the object
    */
   void setPosition(Vector3<float> pos);
   
   /**
    * setAcceleration sets the object's acceleration
    * @param acc the new acceleration
    */
   void setAcceleration(Vector3<float> acc);
   
   /**
    * setForward sets the object's forward vector
    * @ param forward the new forward vector
    */
   void setForward(Vector3<float> forward);
   
   /**
    * setBounds changes the object's bounding volume radius
    * @param f the new volume radius
    */
   void setBounds(float f);
   
   /**
    * getLocation gets the object's current location
    * @return the object's current location
    */
   const Vector3<float> getPosition() const;
   
   /**
    * getForward returns the object's forward vector
    */
   const Vector3<float> getForward() const;
   
   /**
    * getUp returns the object's up vector
    */
   const Vector3<float> getUp() const;
   
   /**
    * doCollision determines the action that the ship should take
    * based on what it has collided with
    * @param other the object being collided with
    */
   virtual void doCollision(GameObject & other) = 0;
   
   /**
    * collidesWith determines if the object has collided with 'other'
    * @param other the object to check against
    */
   virtual bool collidesWith(const GameObject & other) const; 
   
   /**
    * getBoundingSphere returns a pointer to the object's bounding sphere
    */
   const BoundingSphere * getBoundingSphere() const;
   
   /**
    * isAlive deters whether or not the object is still alive
    */
   bool isAlive();
   
   //const BoundingHeirarchy getBounds();
protected:
   int m_health;
   vec3 m_position;
   vec3 m_velocity;
   vec3 m_forward;
   vec3 m_acceleration;
   vec3 m_up;
   
   bool m_alive;
   
private:
   static const float defaultBoundingRadius;
   const BoundingSphere * m_boundingSphere; 
   // BoundingHeirarchy bounds;
};

#endif
