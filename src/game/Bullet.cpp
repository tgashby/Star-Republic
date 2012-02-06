#include "Bullet.h"

Bullet::Bullet(string fileName, string textureName, Modules *modules, 
	       Vector3<float> pos, Vector3<float> forw) 
  : Object3d(), position(0,0,0), forward(0,1,0)
{
  position = pos;
  forward = forw;
}

Bullet::~Bullet() 
{
  
}

void Bullet::tic(uint64_t time)
{
  mat4 posMatrix;
  
  //Add position to global velocity
  position = position + (forward * time * BULLET_VELOCITY);
  //UPDATE VISUAL POSITION
}

Vector3<float> Bullet::getPosition() {
  return position;
}

Vector3<float> Bullet::getForward() {
  return forward;
}
