#include "Reticle.h"

#define SCALE1 2
#define SCALE2 2.5
#define SCALE3 4
#define TRANS1 -100
#define TRANS2 -70 
#define TRANS3 -40

Reticle::Reticle(string fileName, string textureName, Modules *modules, Player *p) 
   :  Object3d()
{
   player = p;

   m_mesh1 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh1);
   mat4 modelMtx1 = mat4::Scale(SCALE1) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getAimForward()), p->getAimUp(), p->getPosition() - p->getAimForward() * TRANS1);
   m_mesh1->setModelMtx(modelMtx1);

   m_mesh2 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh2);
   mat4 modelMtx2 = mat4::Scale(SCALE2) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getAimForward()), p->getAimUp(), p->getPosition() - p->getAimForward() * TRANS2);
   m_mesh2->setModelMtx(modelMtx2);

   m_mesh3 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh3);
   mat4 modelMtx3 = mat4::Scale(SCALE3) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getAimForward()), p->getAimUp(), p->getPosition() - p->getAimForward() * TRANS3);
   m_mesh3->setModelMtx(modelMtx3);
}

Reticle::~Reticle()
{
   
}


void Reticle::tic(uint64_t time)
{
   mat4 modelMtx1 = mat4::Scale(SCALE1) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getAimForward()), player->getAimUp(), player->getPosition() - player->getAimForward() * TRANS1);
   m_mesh1->setModelMtx(modelMtx1);

   mat4 modelMtx2 = mat4::Scale(SCALE2) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getAimForward()), player->getAimUp(), player->getPosition() - player->getAimForward() * TRANS2);
   m_mesh2->setModelMtx(modelMtx2);

   mat4 modelMtx3 = mat4::Scale(SCALE3) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getAimForward()), player->getAimUp(), player->getPosition() - player->getAimForward() * TRANS3);
   m_mesh3->setModelMtx(modelMtx3);
}
