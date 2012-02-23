#ifndef StarRepub_EnemyShip_h
#define StarRepub_EnemyShip_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include "Flyer.h"
#include "Enemy.h"
#include "Mesh.h"
#include "Enemy.h"
#include <assert.h>
#include <cmath>
#include "Explodeable.h"
#include "Missile.h"

const float PATHVELOCITY = 4.2f;
const float AIMVELOCITY = 0.3f;
const float x_SCALAR = 0.0005f; 
const float y_SCALAR = 0.0005f; 
const float mODEL_SCALE = 0.05f;
const float ROTATE_CONSTANT = 90;

/**
 * EnemyShip represents a shy enemy flyer who will dodge based on where
 * the player is currently aiming
 */
class EnemyShip : public Explodeable, public Flyer, public Enemy {
public:
   /**
    * EnemyShip construct, takes 2 strings representing the object file path 
    * and the texture file path respectively, a Modules pointer, and a 
    * player reference.
    * @param fileName the object file, i.e. the model
    * @param textureName the texture file
    * @param modules the Modules pointer
    * @param p the player reference to pass to Enemy
    */
   EnemyShip(string fileName, string textureName, Modules *modules, Player *p);
   ~EnemyShip();
   
   /**
    * tic updates the player ship based on time passed and the player
    * @param time the number of milliseconds passed since the last update
    */
   void tic(uint64_t time);
   
   /**
    * setBearing changes where the enemy ship is facing
    * @param headPos the new head position
    * @param headUp the new up vector for the heading
    */
   void setBearing(Vector3<float> headPos, Vector3<float> headUp);
   
   /**
    * getSide returns the side vector of the ship
    */
   Vector3<float> getSide();
   
   /**
    * getScaredSide returns the vector the ship will head based on where the player is aiming
    */
   Vector3<float> getScaredSide();
   
   /**
    * calculateSide determines the side vector based on the current position
    */
   void calculateSide();
   
   /**
    * updateVelocity does absolutely nothing for the enemy ship
    */
   void updateVelocity(float diffX, float diffY);
   
   /**
    * getAimForward returns the direction the ship is aiming
    */ 
   Vector3<float> getAimForward();
   
   /**
    * getAimUp returns the up vector of the direction the ship is aiming
    */
   Vector3<float> getAimUp();
   
   /**
    * doCollision determines the action that the ship should take
    * based on what it has collided with
    * @param other the object being collided with
    */
   void doCollision(GameObject & other); 
   
   /**
    * shouldFire slows down the enemy's rate of fire so that the player isn't
    * dodging a constant stream of bullets
    */
   bool shouldFire();
   
   /**
    * getLeftCannonPos returns the position of the left cannon, used for 
    * where to initiate bullets
    */
   Vector3<float> getLeftCannonPos();
   
   /**
    * getRightCannonPos returns the position of the right cannon, used for
    * where to initiate bullets
    */
   Vector3<float> getRightCannonPos();

   /**
    * getPosition is required since otherwise it is ambiguous.
    */
   vec3 getPosition();
private:
   Mesh *m_mesh;
   Vector3<float> side;
   Vector3<float> dpos;
   float prevAngle;
   float currentAngle;
   bool dodging;
   int dodgedir, dodgecounter;

   bool firing;
   uint64_t firingTimer;
};

#endif
