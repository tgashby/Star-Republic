#include "Missile.h"

Missile::Missile(string fileName, string textureName, Modules* modules,
		vec3 pos_offset, vec3 forw, vec3 up,
		 GameObject* parent, GameObject* target) 
  : m_lifetime(0), Explodeable(pos_offset, modules),
    GameObject(parent->getPosition() + pos_offset, _MISSILE_RADIUS) {

   m_origPos = parent->getPosition() + pos_offset;

   m_position = m_origPos;
   m_forward = forw.Normalized();
   m_up = up.Normalized();
   
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   m_exhaustMesh = new Mesh("models/missileexhaust.obj", "textures/test4.bmp", modules);
   m_exhaustMesh->setShaderType(SHADER_BLOOM);
   m_meshList.push_back(m_exhaustMesh);

   mat4 modelMtx = mat4::Magic(m_forward, m_up, m_position);
   m_mesh->setModelMtx(modelMtx);
   m_exhaustMesh->setModelMtx(modelMtx);

   m_parent = parent;
   m_target = target;

   m_archVecPos1 = m_parent->getPosition() + (m_forward * 80) + (m_up * 10);
   m_archVecPos2 = m_parent->getPosition() - (m_forward * 40) + (m_up * 80); 
   m_archVecPos3 = m_position + ((target->getPosition() - m_position) / 2) 
      + (m_up * _MISSILE_ARC_HEIGHT);
   m_aimPos = m_archVecPos3;

}

Missile::~Missile() {
   
}

int Missile::getTotalTime() {
  return m_tottime;
}

void Missile::tic(uint64_t time) {
   mat4 modelMtx;
   m_tottime += time;

   if (!m_detonated) {
      m_lifetime += time;
      if (m_lifetime >= _MISSILE_REACH_DEST_TIME) {
	 m_lifetime = _MISSILE_REACH_DEST_TIME;
	 m_detonated = true;
	 m_mesh->setVisible(false);
	 setExplosionPosition(m_position);
      }
      
      //CHANGE TO ACCOUNT FOR SIDES
      m_archVecPos1 = m_parent->getPosition() + (m_forward * 40) + (m_up * 20);
      m_archVecPos2 = m_parent->getPosition() - (m_forward * 40) + (m_up * 60);
      m_archVecPos3 = m_parent->getPosition() + 
	 ((m_target->getPosition() - m_parent->getPosition()) / 2) 
	 + (m_up * _MISSILE_ARC_HEIGHT);
      
      m_aimPos = m_archVecPos3 + 
	 ((m_target->getPosition() - m_archVecPos3) 
	  * (m_lifetime / _MISSILE_REACH_DEST_TIME));

      //DEBUG

      if (m_lifetime < _MISSILE_ARCH_VEC_1_TIME) {
	 m_position = m_origPos + ((m_archVecPos1 - m_origPos)
	  * (m_lifetime / _MISSILE_ARCH_VEC_1_TIME));
      }
      else if (m_lifetime < _MISSILE_ARCH_VEC_1_TIME + 
	       _MISSILE_ARCH_VEC_2_TIME) {
	 m_position = m_archVecPos1 + ((m_archVecPos2 - m_archVecPos1)
	  * ((m_lifetime - _MISSILE_ARCH_VEC_1_TIME) / _MISSILE_ARCH_VEC_2_TIME));
      }
      else {
	 //CHANGE
	 m_position = m_archVecPos2 + ((m_aimPos - m_archVecPos2) 
	  * ((m_lifetime - _MISSILE_ARCH_VEC_1_TIME - _MISSILE_ARCH_VEC_2_TIME)
	  / _MISSILE_ARCH_VEC_3_TIME));
      }
      
      //WILL BECOME NAN AFTER IT HITS, NOT GOOD BUT OK FOR NOW
      m_forward = (m_target->getPosition() - m_position).Normalized();
      
      modelMtx = mat4::Magic(m_forward, m_up, m_position);
      m_mesh->setModelMtx(modelMtx);
      modelMtx = mat4::Rotate(-90, vec3(0,0,1)) * modelMtx;
      m_exhaustMesh->setModelMtx(modelMtx);
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
      m_mesh->setVisible(false);
      setExplosionPosition(m_position);
   }
}
