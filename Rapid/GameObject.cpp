#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glut.h>
#endif

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
void GameObject::collideWith(GameObject* collided)
{
}

void GameObject::update(float dt)
{
   this->position->X += this->velocity->X * dt;
   this->position->Y += this->velocity->Y * dt;
   this->position->Z += this->velocity->Z * dt;
}

SVector3* GameObject::getPosition()
{
   return this->position;
}
