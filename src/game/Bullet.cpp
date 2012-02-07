#include "Bullet.h"

Bullet::Bullet(string fileName, string textureName, Modules *modules, 
	       Vector3<float> pos, Vector3<float> forw, Vector3<float> up) 
  : Object3d(), position(0,0,0), forward(0,0,1), up(0, 1, 0)
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
  
}

Vector3<float> Bullet::getPosition() {
  return position;
}

Vector3<float> Bullet::getForward() {
  return forward;
}
