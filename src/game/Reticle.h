#ifndef StarRepub_Reticle_h
#define StarRepub_Reticle_h

#include "Interfaces.h"
#include "Player.h"
#include "Object3d.h"
#include "Mesh.h"
#include <assert.h>
#include <cmath>
#define VELOCITY 0.02f
#define X_SCALAR 0.0005f
#define Y_SCALAR 0.0005f
#define MODEL_SCALE 0.8f

class Reticle : public Object3d {
public:
   Player *player;
   Mesh *m_mesh1, *m_mesh2, *m_mesh3;   

   Reticle(string fileName, string textureName, Modules *modules, Player *p);
   ~Reticle();
   void tic(uint64_t time);
};


#endif
