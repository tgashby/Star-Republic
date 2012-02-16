#include "Missile.h"

Missile::Missile(string fileName, string textureName, Modules* modules,
		vec3 pos, vec3 forw, vec3 up,
		 GameObject* parent, GameObject* target) : m_lifetime(0), Explodeable(pos, modules) {
   m_origPos = pos;

   m_position = pos;
   m_forward = forw.Normalized();
   m_up = up.Normalized();
   
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx = mat4::Magic(m_forward, m_up, m_position);
   m_mesh->setModelMtx(modelMtx);

   m_parent = parent;
   m_target = target;

   m_archVecPos = m_position + ((target->getPosition() - m_position) / 2) 
      + (m_up * _MISSILE_ARC_HEIGHT);
   m_aimPos = m_archVecPos;

//   cerr << "Missile created at pos : " << m_position.x << " " << m_position.y << " " << m_position.z << "\n";
}

Missile::~Missile() {
   delete m_mesh;
}

void Missile::tic(uint64_t time) {
   mat4 modelMtx;

   if (!m_detonated) {
      m_lifetime += time;
      if (m_lifetime > _MISSILE_REACH_DEST_TIME) {
	 m_lifetime = _MISSILE_REACH_DEST_TIME;    
      }
      
      m_archVecPos = m_parent->getPosition() + ((m_target->getPosition() - m_parent->getPosition()) / 2) + (m_up * _MISSILE_ARC_HEIGHT);
      
      m_aimPos = m_archVecPos + ((m_target->getPosition() - m_archVecPos) 
				 * (m_lifetime / _MISSILE_REACH_DEST_TIME));
      //DEBUG
      //m_position = m_aimPos;
      m_position = m_origPos + ((m_aimPos - m_origPos) * (m_lifetime / _MISSILE_REACH_DEST_TIME));
      //   cerr << "Missile position is now : " << m_position.x << ", " << m_position.y << ", " << m_position.z << "\n";
      
      //WILL BECOME NAN AFTER IT HITS, NOT GOOD BUT OK FOR NOW
      m_forward = (m_target->getPosition() - m_position).Normalized();
      
      modelMtx = mat4::Magic(m_forward, m_up, m_position);
      m_mesh->setModelMtx(modelMtx);
   }
   else {
      explosionTic(time);
   }
}
   
void Missile::doCollision(GameObject &other) {
   //ADD COLLISION CODE HERE
   if (typeid(other) != typeid(Player) && typeid(other) != typeid(Bullet)
       && typeid(other) != typeid(Missile)) {
      m_detonated = true;
      setExplosionPosition(m_position);
   }
}
