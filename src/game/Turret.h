//
//  Turret.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Turret_h
#define StarRepub_Turret_h

#include "Interfaces.h"
#include "Object3d.h"
#include "Player.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Mesh.h"
#include "Explodeable.h"
#include "Missile.h"

#define _TURRET_DEFAULT_EXPLOSION_RADIUS 40.0f
#define _TURRET_SCALE 150.0f

/**
 * Turret class for turrets
 */
class Turret : public Enemy, public GameObject, 
  public Explodeable {
public:
   /**
    * Turret Constructor with a Player reference, 6 strings representing the
    * models and textures of the turret (it's in 3 pieces), and a Modules
    * pointer
    * @param player the player reference, so the turrets know where to aim
    * @headName path to file for head model
    * @headTexture path to file for head texture
    * @midName path to file for middle model
    * @midTexture path to file for middle texture
    * @footName path to file for foot model
    * @footTexture path to file for foot texture
    * @param modules the Modules pointer
    */
   Turret(Player* player, Modules *modules);
   
   ~Turret();
   
   /**
    * tic updates the turret based on the time passed
    * @param time the time passed since the last update, in milliseconds
    */
   void tic(uint64_t time);
   
   /**
    * doCollision determines the action that the player should take
    * based on what it has collided with
    * @param other the object being collided with
    */
   void doCollision(GameObject & other);
   
   /**
    * getHeadPosition returns the Turret's head position, used for
    * shooting bullets from the right place
    */
   Vector3<float> getHeadPosition();
   
   /**
    * shouldFire regulates the turret's firing rate
    */
   bool shouldFire();

   vec3 getPosition();
     
   bool viewCull(vector<vec4> *planes);
   
private:
   Mesh *m_headMesh;
   Mesh *m_midMesh;
   Mesh *m_footMesh;
   
   bool firing;
   uint64_t firingTimer;
   
   void collideWith(Bullet& bullet);
   void collideWith(Player& player);
   void collideWith(Enemy& enemy);
   void collideWith(Missile& missile);
   int flashtimer;
//   std::list<Bullet*> m_bullets;
};

#endif
