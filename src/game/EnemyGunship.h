#ifndef StarRepub_EnemyGunship_h
#define StarRepub_EnemyGunship_h

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

/**
 * EnemyShip represents a shy enemy flyer who will dodge based on where
 * the player is currently aiming
 */
class EnemyGunship : public Flyer, public Enemy, public Explodeable {
 public:
   /**
    * EnemyShip construct, takes 2 strings representing the object file path 
    * and the texture file path respectively, a Modules pointer, and a 
    * player reference.
    * @param fileName the object file, i.e. the model
    * @param turretFileName1 an object file for the turretbase
    * @param turretFileName2 an object file for the turrethead
    * @param textureName the texture file
    * @param modules the Modules pointer
    * @param p the player reference to pass to Enemy
    */
   EnemyGunship(string fileName, string turretFileName1, string turretFileName2,
      string textureName, Modules *modules, Player &p);
   ~EnemyGunship();
   
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
   
   /*
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
    * -left cannon
    */
   bool shouldFire1();

   /**
    * shouldFire slows down the enemy's rate of fire so that the player isn't
    * dodging a constant stream of bullets
    * -right cannon
    */
   bool shouldFire2();
   
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
    * getPosition returns the ships position.  It is to deal with the problem
    * of ambiguous getPosition calls.
    */
   vec3 getPosition();

private:
   Mesh *m_mesh;
   /** two turrets, each consisting of 2 parts **/
   Mesh *m_turretbasemesh1, *m_turretbasemesh2;
   Mesh *m_turretheadmesh1, *m_turretheadmesh2;
   Vector3<float> side;
   Vector3<float> dpos;
   float prevAngle;
   float currentAngle;

   bool firing1, firing2;
   uint64_t firingTimer1, firingTimer2;
   uint64_t motionTimer;
   // for moving 
   int xdir, ydir, pxdir, pydir;
};

#endif
