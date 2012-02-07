#pragma once
#define BULLET_VELOCITY 10

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"

class Bullet : public Object3d {
 public:
  Bullet(string fileName, string textureName, Modules *modules, 
	 Vector3<float> pos, Vector3<float> forw, Vector3<float> up); 
  ~Bullet();

  void tic(uint64_t time);
  Vector3<float> getPosition();
  Vector3<float> getForward();

 private:
  Vector3<float> position;
  Vector3<float> forward;
  Vector3<float> up;
};
