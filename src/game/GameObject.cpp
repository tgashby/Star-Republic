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
GameObject::GameObject(Coordinate startPoint, 
                       Coordinate startVelocity, 
                       Angle startHeading, 
                       BoundingStructure boundingStructure){
   m_location = startPoint;
   m_direction = startHeading;
   m_velocity = startVelocity;
   m_boundingStructure = boundingStructure;
   
   
   
   // Just to put a space ship on the screen for now.
   // Derived classes should provoide their own meshes.
   m_meshList = list<IMesh *>(0);
   Mesh *mesh = new Mesh(DEFAULT_MESH, DEFAULT_TEXTURE);
   m_meshList.push_back(mesh);
}


GameObject::~GameObject() {
   list<IMesh *>::iterator mesh;
   for (mesh = m_meshList.begin(); mesh != m_meshList.end(); ++mesh) {
      delete *mesh;
   }
}


/**
 * tic - Runs once per game loop to update the game object's location and
 * heading.
 * @param int dt - The amount of time it took to draw the previous frame.
*/
void GameObject::tic(int dt){
   m_location = m_location + m_velocity*dt;
   m_direction = m_direction + m_angularVelocity*dt;
}

/**
 * setVelocity - sets the speed and direction of a GameObject. 
 * @param velocity - the new velocity
 * @returns true if the new velocity is valid for this object. For this
 * base class, that will always be true, however specific classes of 
 * game objects might want to enforce limits to their velocities. 
 */
bool GameObject::setVelocity(const Coordinate velocity){
   m_velocity = velocity;
   return true;
}

/**
 * setAngularVelocity - sets the rate at which GameObject's heading changes. 
 * @param angularVelocity - the new angular velocity
 * @returns true if the new angular velocity is valid for this object. For this
 * base class, that will always be true, however specific classes of 
 * game objects might want to enforce limits to their angular velocities.  
 */
bool GameObject::setAngularVelocity(const Angle angularVelocity){
   m_angularVelocity = angularVelocity;
   return true;
}

/**
 * getMeshes - gets a pointer to a stl list of IMeshs.
 */

list<IMesh *>* GameObjcet::getMeshes() {
   return m_meshList;
}

/**
 * getLocation gets the object's current location
 * @return the object's current location
 */
const Coordinate getLocation(){
   return m_location;
}

/**
 * getBoundingStructure gets the object's current location
 * @return the object's current location
 */
const BoundingStructure getBoundingStructure(){
   return m_boundingStructure;
}

