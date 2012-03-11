
#include "Objective.h"

#define _OBJECTIVE_DEFAULT_EXPLOSION_RADIUS 300.0f
#define _OBJECTIVE_DEFAULT_MODEL_SCALE 70.0f

Objective::Objective(string fileName, string textureName, Modules *modules,
		     vec3 pos, vec3 forw, vec3 up) 
   : Explodeable (pos, _OBJECTIVE_DEFAULT_EXPLOSION_RADIUS, modules) {
   m_up = up;
   m_position = pos;
   m_forward = forw;

   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   m_mesh->setScale(_OBJECTIVE_DEFAULT_MODEL_SCALE);
   m_mesh->setModelMtx(mat4::Magic(m_forward, m_up, m_position));
   m_mesh->setShaderType(SHADER_BLOOM);
}

Objective::~Objective() {

}

void Objective::tic(uint64_t time) {
   //MAY ADD CODE TO MAKE IT ALTERNATE FROM GLOWING TO NON-GLOWING
   //(PROBABLY WOULD BE DONE IN THE SHADER THOUGH)
   if (!isAlive()) {
      //cout << "Exploding???\n";
      explosionTic(time);
   }
}

void Objective::doCollision(GameObject & other) {
   if (typeid(other) == typeid(Missile)) {
      m_health -= 50;
   }
   
   if (typeid(other) == typeid(Bullet)) {
      m_health -= 10;
   }

   if (m_health <= 0) {
      m_alive = false;
      m_mesh->setVisible(false);
      setExplosionPosition(m_position);
   }
}

vec3 Objective::getPosition() {
   return m_position;
}
