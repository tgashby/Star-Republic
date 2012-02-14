//
//  Explodeable.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/9/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Explodeable_h
#define StarRepub_Explodeable_h

#include <string.h>
#include "Object3d.h"
#include "Mesh.h"
#include "Interfaces.h"

using namespace std;

/*
 * ExplosionState is a simple enum to keep track of what stage
 * the explosion is in
 */
enum ExplosionState 
{
   START = 0,
   MIDDLE = 1,
   END = 2,
   DONE = 3
};

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
    * explode models a simple expanding and contracting mesh
    */
   void explode();
   
   /**
    * tic updates the explosion based on time passed
    * @param dt the time passed since the last update in milliseconds
    */
   void tic(uint64_t dt);
   
   /**
    * setPostition moves the explosion if ever needed
    * @param position the new position for the explosion
    */
   void setPosition(Vector3<float> position);
   
   /**
    * getPosition returns the explosion's current position
    */
   Vector3<float> getPosition() { return m_position; }
   
private:
   Mesh* m_mesh;
   ExplosionState state;
   vec3 m_position;
   uint64_t stateTimer;
};

#endif
