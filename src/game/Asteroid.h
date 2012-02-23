
#ifndef StarRepub_Asteroid_h
#define StarRepub_Asteroid_h

#define _ASTEROID_DEFAULT_RADIUS 50.0f
#define _ASTEROID_DEFAULT_VELOCITY 10.0f

#include "GameObject.h"
#include "Interfaces.h"
#include "Mesh.h"
#include "../engine/Object3d.h"

class Asteroid : public Object3d, public GameObject {
 public:
  Asteroid(string fileName, string textureName, Modules *modules,
	   vec3 pos, vec3 forw, vec3 up);
  ~Asteroid();
  
  void tic(uint64_t time);
  void doCollision(GameObject & other);
 private:
  Mesh* m_mesh;
  
};

#endif
