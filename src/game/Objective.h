
#ifndef StarRepub_Objective_h
#define StarRepub_Objective_h

#include "GameObject.h"
#include "Interfaces.h"
#include "Mesh.h"
#include "Object3d.h"
#include "Explodeable.h"
#include "Missile.h"
#include "Bullet.h"

/**
 * Object represents the world markers that must be destroyed to win.
 * It is very basic, and all checking should be done higher up to see
 * if the player has won.
 */
class Objective : public Explodeable, public GameObject {
 public:

   /**
    * Constructs an Objective using the given mesh/texture
    * Another constructor can be written for a default mesh/texture
    */
   Objective(string fileName, string textureName, Modules *modules,
	     vec3 pos, vec3 forw, vec3 up);

   /**
    * Frees the object from memory
    */
   ~Objective();

   /**
    * Tics the objective.  Doubt we will use at all, unless we decide to
    * have some sort of pulse.
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
};

#endif
