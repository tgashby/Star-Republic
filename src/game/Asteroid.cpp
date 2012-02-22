
#include "Asteroid.h"

#define MODEL_SCALE 10.0f

Asteroid::Asteroid(string fileName, string textureName, Modules *modules,
		   vec3 pos, vec3 forw, vec3 up) 
  : Object3d(), GameObject(pos, _ASTEROID_DEFAULT_RADIUS) {
  m_position = pos;
  m_forward = forw;
  m_up = up;

  m_mesh = new Mesh(fileName, textureName, modules);
  m_meshList.push_back(m_mesh);

  m_mesh->setScale(MODEL_SCALE);
  m_mesh->setModelMtx(mat4::Translate(m_position.x, m_position.y, m_position.z));
}

Asteroid::~Asteroid() {

}

void Asteroid::tic(uint64_t dt) {
  
}

void Asteroid::doCollision(GameObject & other) {
  //DO NOTHING YET
}
