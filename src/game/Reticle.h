#ifndef StarRepub_Reticle_h
#define StarRepub_Reticle_h

#include "Interfaces.h"
#include "Player.h"
#include "Object3d.h"
#include "Mesh.h"
#include <assert.h>
#include <cmath>

class Reticle : public Object3d {
public:
   Player *player;
   Mesh *m_mesh1, *m_mesh2, *m_mesh3;   

   Reticle(string fileName, string textureName, Modules *modules, Player *p);
   ~Reticle();
   void tic(uint64_t time);
};


#endif
