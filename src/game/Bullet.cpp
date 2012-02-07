#include "Bullet.h"

Bullet::Bullet(string fileName, string textureName, Modules *modules, 
	       Vector3<float> pos, Vector3<float> forw, Vector3<float> up) 
  : Object3d(), m_position(0,0,0), m_forward(0,0,1), 
    m_up(0, 1, 0), m_side(1, 0, 0)
{
  m_position = pos;
  m_forward = forw.Normalized();
  m_up = up.Normalized();
  calculateSide();

  m_mesh = new Mesh(fileName, textureName, modules);
  m_meshList.push_back(m_mesh);
  
  mat4 modelMtx = mat4::Magic(m_forward, m_up, m_side) * 
    mat4::Translate(pos.x, pos.y, pos.z);
  m_mesh->setModelMtx(modelMtx);
  
}

Bullet::~Bullet() 
{
  
}

void Bullet::tic(uint64_t time)
{  
  mat4 modelMtx;
  //Add position to global velocity
  m_position = m_position + (m_forward * time * BULLET_VELOCITY);
  calculateSide();

  modelMtx = mat4::Magic(m_forward, m_up, m_side) * 
    mat4::Translate(m_position.x, m_position.y, m_position.z);
  m_mesh->setModelMtx(modelMtx);
}

Vector3<float> Bullet::getPosition() {
  return m_position;
}

Vector3<float> Bullet::getForward() {
  return m_forward;
}

Vector3<float> Bullet::getUp() {
  return m_up;
}

void Bullet::calculateSide()
{
   m_side = m_up.Cross(m_forward).Normalized();
}
