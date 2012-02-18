
#include "Asteroid.h"

Asteroid::Asteroid(string fileName, string textureName, Modules *modules,
		   vec3 pos, vec3 forw, vec3 up) 
  : Object3d(), GameObject(pos, _ASTEROID_DEFAULT_RADIUS) {
  m_position = pos;
  m_forward = forw;
  m_up = up;
}

Asteroid::~Asteroid() {

}

void Asteroid::tic(uint64_t dt) {
  
}

void Asteroid::doCollision(GameObject & other) {

}
