#ifndef StarRepub_Reticle_h
#define StarRepub_Reticle_h

#include "Interfaces.h"
#include "Player.h"
#include "Object3d.h"
#include "Mesh.h"
#include <assert.h>
#include <cmath>
#define X_SCALAR 0.0005f
#define Y_SCALAR 0.0005f
#define MODEL_SCALE 0.8f

/**
 * Reticle class represents the player's recticle
 */
class Reticle : public Object3d {
public:
   /**
    * Recticle construct taking 2 strings representing the recticle's model 
    * and texture files, a Modules pointer, and a pointer to the Player
    * @param fileName the path to the reticle's model
    * @param textureName the path to the reticle's texture
    * @param modules the Modules pointer
    * @param p the Player pointer
    */
   Reticle(string fileName, string textureName, Modules *modules, Player *p);
   ~Reticle();
   
   /**
    * tic updates the reticle
    * @param time the time passed since the last update, in milliseconds
    */
   void tic(uint64_t time);

   /**
    * setVisible changes the reticle to invisible
    * @param visibleStatus - The new visibility status of the reticle
    */
   void setVisible(bool visibleStatus);
   
private:
   Player *player;
   Mesh *m_mesh1, *m_mesh2, *m_mesh3; 
};


#endif
