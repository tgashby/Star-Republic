//
//  PathObject.h
//  StarRepub
//
//  Created by Robert Crosby on 3/13/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_PathObject_h
#define StarRepub_PathObject_h

#define TEXTURE_MOVE_RATE 0.001

#include "Interfaces.h"
#include "GameObject.h"
#include "Object3d.h"
#include "Mesh.h"

class PathObject : public Object3d, public GameObject {
public:
   
   /**
    * Constructs an Objective using the given mesh/texture
    * Another constructor can be written for a default mesh/texture
    */
   PathObject(string fileName, string textureName, Modules *modules,
             vec3 pos, vec3 forw, vec3 up);
   
   /**
    * Frees the object from memory
    */
   ~PathObject();
   
   /**
    * Tics the PathObject.
    */
   void tic(uint64_t time);
   
   /**
    * Collision code for the objective.  Only hurt by bullets and missiles
    */
   void doCollision(GameObject & other);
   
   /**
    * getPosition is required because of conflicting functions
    */
   vec3 getPosition();
   
private:
   /**
    * The object's mesh
    */
   Mesh* m_mesh;
   float m_textureOffset;
};

#endif
