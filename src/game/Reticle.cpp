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

   /** Load the Mesh **/
   m_mesh1 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh1);
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx1 = mat4::Scale(SCALE1) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getAimForward()), p->getAimUp(), p->getPosition() - p->getAimForward() * TRANS1);
   m_mesh1->setModelMtx(modelMtx1);

   /** Load the Mesh **/
   m_mesh2 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh2);
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx2 = mat4::Scale(SCALE2) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getAimForward()), p->getAimUp(), p->getPosition() - p->getAimForward() * TRANS2);
   m_mesh2->setModelMtx(modelMtx2);

   /** Load the Mesh **/
   m_mesh3 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh3);
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx3 = mat4::Scale(SCALE3) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getAimForward()), p->getAimUp(), p->getPosition() - p->getAimForward() * TRANS3);
   m_mesh3->setModelMtx(modelMtx3);
}

Reticle::~Reticle()
{
   
}


void Reticle::tic(uint64_t time)
{
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx1 = mat4::Scale(SCALE1) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getAimForward()), player->getAimUp(), player->getPosition() - player->getAimForward() * TRANS1);
   m_mesh1->setModelMtx(modelMtx1);

   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx2 = mat4::Scale(SCALE2) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getAimForward()), player->getAimUp(), player->getPosition() - player->getAimForward() * TRANS2);
   m_mesh2->setModelMtx(modelMtx2);

   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx3 = mat4::Scale(SCALE3) * mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getAimForward()), player->getAimUp(), player->getPosition() - player->getAimForward() * TRANS3);
   m_mesh3->setModelMtx(modelMtx3);
}

void Reticle::setVisible(bool visibleStatus) {
   m_mesh1->setVisible(visibleStatus);
   m_mesh2->setVisible(visibleStatus);
   m_mesh3->setVisible(visibleStatus);
}
