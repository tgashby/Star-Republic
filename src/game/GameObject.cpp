#include "GameObject.h"
#include "Mesh.h"

#define DEFAULT_MESH "spaceship.obj"
#define DEFAULT_TEXTURE "blank.bmp"


/**
 * GameObject base class definition. All movable in-game objects are derived
 * from this class
 */

/**
 * GameObject Constructor. Used for creating a new game object from scrach.
 * @param startPoint - The point where the object is created
 * @param startHeading - The direction that the model is pointed when it is
 * created.
 * @param startVelocity - The speed and direction of the object when it is 
 * created.
 * @param boundingStructure - The game object's bounding structure
 */ 
GameObject::GameObject(vec3 startPos, vec3 startVelocity, 
                       vec3 forwardVec, vec3 accelerationVec, 
                       vec3 upVec)
{
   m_position = startPos;
   m_velocity = startVelocity;
   m_forward = forwardVec;
   m_acceleration = accelerationVec;
   m_up = upVec;
}

GameObject::GameObject()
{
   m_position = vec3(0,0,0);
   m_velocity = vec3(0,0,0);
   m_forward = vec3(0,0,-1);
   m_acceleration = vec3(0,0,0);
   m_up = vec3(0,1,0);
}


GameObject::~GameObject() {}


/**
 * tic - Runs once per game loop to update the game object's location and
 * heading.
 * @param int dt - The amount of time it took to draw the previous frame.
*/
void GameObject::tic(uint64_t td)
{
   
}

/**
 * setVelocity - sets the speed and direction of a GameObject. 
 * @param velocity - the new velocity
 * @returns true if the new velocity is valid for this object. For this
 * base class, that will always be true, however specific classes of 
 * game objects might want to enforce limits to their velocities. 
 */
void GameObject::setVelocity(const Vector3<float> velocity)
{
   m_velocity = velocity;
}

void GameObject::setUp(Vector3<float> upVal)
{
   m_up = upVal;
}

void GameObject::setPosition(Vector3<float> pos)
{
   m_position = pos;
}

void GameObject::setAcceleration(Vector3<float> acc)
{
   m_acceleration = acc;
}

/**
 * getLocation gets the object's current location
 * @return the object's current location
 */
const Vector3<float> GameObject::getPosition()
{
   return m_position;
}

const Vector3<float> GameObject::getForward()
{
   return m_forward;
}

const Vector3<float> GameObject::getUp()
{
   return m_up;
}
