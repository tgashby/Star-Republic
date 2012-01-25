#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
//#include "GLSL_helper.h"
#include <math.h>
#include "Enemy.h"
#include "GameObject.h"

GameObject::GameObject(SVector3* pos, SVector3* vel, CMesh * mod, float size)
{
   position = pos;
   velocity = vel;
   model = mod;
   this->size = size;
}

GameObject::~GameObject()
{
}

void draw() 
{
}
void collideWith(GameObject collided)
{
}

void GameObject::update(float dt)
{
   this->position->X += this->velocity->X * dt;
   this->position->Y += this->velocity->Y * dt;
   this->position->Z += this->velocity->Z * dt;
}
