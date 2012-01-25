#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"
#include "Util/SVector3.h"

#endif

class GameObject{
public:
   CMesh * model;
   SVector3* position;
   SVector3* velocity;
   
   GameObject (SVector3* pos, SVector3* vel, CMesh * mod);
   ~GameObject();
   void draw();
   void collideWith(GameObject collided);
   void update(float time);
};
   
