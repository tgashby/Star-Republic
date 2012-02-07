//
//  Bullet.h
//  StarRepub
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#pragma once
#define BULLET_VELOCITY 0.15f

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
  Vector3<float> getUp();
  void calculateSide();

 private:
  Mesh* m_mesh;
  Vector3<float> m_position;
  Vector3<float> m_forward;
  Vector3<float> m_up;
  Vector3<float> m_side;
};
