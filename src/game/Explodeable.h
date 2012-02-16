//
//  Explodeable.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/9/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Explodeable_h
#define StarRepub_Explodeable_h

//Adjustments to be done
#define _DEF_EXPLOSION_ANIMATION_TIME 750.0f
#define _DEF_EXPLOSION_RADIUS 20.00f

#include <string.h>
#include "Object3d.h"
#include "Mesh.h"
#include "Interfaces.h"

using namespace std;

/**
 * Explodable is an interface for things that can explode
 */
class Explodeable : public Object3d {
public:
   /**
    * Explodeable constructor that determines the position of an explosion
    * @param position the position of the explosion
    * @param modules the Modules pointer
    */
   Explodeable(vec3 position, Modules* modules);

   /**
    * Creates a new explosion with more aspects to set.
    * @param position - the position of the explosion
    * @param duration - the duration that the explosion should take
    * @param modules - the Modules pointer
    */
   Explodeable(vec3 position, int duration, float radius, Modules* modules);
   
   /**
    * tic updates the explosion based on time passed
    * @param dt the time passed since the last update in milliseconds
    */
   void tic(uint64_t dt);
   
   /**
    * getPosition returns the explosion's current position
    * @return Returns the position of the explosion
    */
   Vector3<float> getPosition() { return m_explosionPos; }

   /**
    * setPosition sets the explosion to a new location
    * @param pos The new location for the explosion
    */
   void setExplosionPosition(vec3 pos);

   /**
    * explosionTic Tics 
    */
   void explosionTic(uint64_t dt);

private:
   /**
    * The explosion's corresponding mesh
    */
   Mesh* m_explosionMesh1;

   /**
    * The position of the explosion
    */
   vec3 m_explosionPos;

   /**
    * The time since the explosion's creation
    */
   float m_stateTimer;

   /**
    * The time the explosion takes to complete
    */
   float m_duration;

   /**
    * The radius of the explosion
    */
   float m_radius;
};

#endif
