//
//  Missle.h
//  StarRepub
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Missile_h
#define StarRepub_Missile_h

//the time it takes for the missile to reach its destination
#define _MISSILE_REACH_DEST_TIME 1500
#define _MISSILE_ARC_HEIGHT 500
#define _MISSILE_RADIUS .4f
#define _MISSILE_ARCH_VEC_1_TIME 500
#define _MISSILE_ARCH_VEC_2_TIME 500
#define _MISSILE_ARCH_VEC_3_TIME 500

#include "GameObject.h"
#include "Interfaces.h"
#include "Mesh.h"
#include "../engine/Object3d.h"
#include "Explodeable.h"
#include "Player.h"
#include "Bullet.h"

class Missile : public Explodeable, public GameObject {
 public:

  /**
   * Constructor for a missile.
   * @param fileName - The filename of the mesh used for the missile.
   * @param textureName - The name of the file with the texture for the missile
   * @param module - The game's modules
   * @param pos - The initial position of the missile
   * @param forw - The forward vector of the missile
   * @param up - The up vector of the missile
   * @param parent - The missile's parent, used to ensure it isn't killed by its own missile.
   * @param target - The target of the missile; it follows them based on their position
   */
   Missile(string fileName, string textureName, Modules *modules,
	   vec3 pos_offset, vec3 forw, vec3 up,
	   GameObject* parent, GameObject* target);

   /**
    * The default deconstructor for a missile
    */
   ~Missile();

   /**
    * Changes the missile's locationb based on the time between screens
    * @param time - The time that has passed since the previous screen
    */
   void tic(uint64_t time);

   /**
    * Gives the current position of the missile as a vector
    * @return The current position of the missile
    */
   vec3 getPosition();

   /**
    * Gives the forward vector of the missile
    * @return The forward vector of the missile
    */
   vec3 getForward();

   /** 
    * Gives the up vector of the missile
    * @return The up vector of the missile
    */
   vec3 getUp();
   
   /**
    * The missile's collision handling code
    * In general, the missile dies.
    */
   void doCollision(GameObject &other);

 private:
   
   /**
    * The missile's mesh
    */
   Mesh* m_mesh;

   /**
    * How long the missile has been active
    */
   float m_lifetime;

   /**
    * The vector used to form an arc above the player
    */
   vec3 m_archVecPos1;
   vec3 m_archVecPos2;
   vec3 m_archVecPos3;

   /**
    * The current location between the m_archVecPos and the target that the
    * missile is looking at
    */
   vec3 m_aimPos;

   /**
    * The parent of the missile, so we don't get killed by our own missiles
    */
   GameObject* m_parent;

   /**
    * The missile's locked target
    */
   GameObject* m_target;
   
   /**
    * The original position of the missile.
    * Allows the missile to fall behind the player
    */
   vec3 m_origPos;

   /**
    * Whether or not the missile has detonated
    */
   bool m_detonated;
};

#endif
