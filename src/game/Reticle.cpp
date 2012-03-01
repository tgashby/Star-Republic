#include "Reticle.h"

#define SCALE1 4
#define SCALE2 5
#define SCALE3 8
#define TRANS1 -120
#define TRANS2 -90 
#define TRANS3 -50

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
   mat4 modelMtx1 = mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getMagneticForward()), p->getAimUp(), p->getPosition() - p->getMagneticForward() * TRANS1);
   m_mesh1->setModelMtx(modelMtx1);
   m_mesh1->setScale(SCALE1);
   m_mesh1->setShaderType(SHADER_NO_LIGHT);

   /** Load the Mesh **/
   m_mesh2 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh2);
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx2 = mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getMagneticForward()), p->getAimUp(), p->getPosition() - p->getMagneticForward() * TRANS2);
   m_mesh2->setModelMtx(modelMtx2);
   m_mesh2->setScale(SCALE2);
   m_mesh2->setShaderType(SHADER_NO_LIGHT);

   /** Load the Mesh **/
   m_mesh3 = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh3);
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx3 = mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(p->getMagneticForward()), p->getAimUp(), p->getPosition() - p->getMagneticForward() * TRANS3);
   m_mesh3->setModelMtx(modelMtx3);
   m_mesh3->setScale(SCALE3);
   m_mesh3->setShaderType(SHADER_NO_LIGHT);
}

Reticle::~Reticle()
{
   
}


void Reticle::tic(uint64_t time)
{
   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx1 = mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getMagneticForward()), player->getAimUp(), player->getPosition() - player->getMagneticForward() * TRANS1);
   m_mesh1->setModelMtx(modelMtx1);

   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx2 = mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getMagneticForward()), player->getAimUp(), player->getPosition() - player->getMagneticForward() * TRANS2);
   m_mesh2->setModelMtx(modelMtx2);

   /** Create the model matrix from a constant scale and rotate (different for each 
    *  of the three reticle pieces), the player's aim-forward and aim-up and 
    *  the players position modified by a scalar times the player's aim-forward **/
   mat4 modelMtx3 = mat4::Rotate(90, vec3(1, 0, 0)) *
      mat4::Magic(-(player->getMagneticForward()), player->getAimUp(), player->getPosition() - player->getMagneticForward() * TRANS3);
   m_mesh3->setModelMtx(modelMtx3);
}

void Reticle::setVisible(bool visibleStatus) {
   m_mesh1->setVisible(visibleStatus);
   m_mesh2->setVisible(visibleStatus);
   m_mesh3->setVisible(visibleStatus);
}
