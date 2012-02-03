#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"

class Player : public Object3d {
public:
   Player(string fileName, string textureName, Modules *modules);
   ~Player();
private:
   int health;
   Mesh *m_mesh;
};

#endif
