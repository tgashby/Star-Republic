#include "Player.h"
#include "Mesh.h"

Player::Player(string fileName, string textureName, Modules *modules) 
   : Object3d() 
{
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx = mat4::Translate(0, 0, -100);
   modelMtx = mat4::Rotate(90, vec3(0, 1, 0)) * modelMtx;

   m_mesh->setModelMtx(modelMtx);
}
Player::~Player()
{

}

