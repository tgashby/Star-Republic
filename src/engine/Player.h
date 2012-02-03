#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include "Mesh.h"

class Player : public Object3d {
public:
   Player(string fileName, string textureName, Modules *modules);
   ~Player();
private:
   int health;
   Mesh *m_mesh;
};

#endif
