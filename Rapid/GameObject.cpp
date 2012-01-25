#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
//#include "GLSL_helper.h"
#include <math.h>
#include "Enemy.h"
#include "GameObject.h"

GameObject::GameObject(SVector3 pos, SVector3 vel, CMesh * mod)
{
   this->position = pos;
   this->velocity = vel;
   this->model = mod;
}

GameObject::~GameObject()
{
}

GameObject::Update(float dt)
{
   this->position.X += this->velocity.X * dt;
   this->position.Y += this->velocity.Y * dt;
   this->position.Z += this->velocity.Z * dt;
}
