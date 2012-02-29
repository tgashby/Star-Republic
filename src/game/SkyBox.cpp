#include "SkyBox.h"

#define BOXSCALE 5000
  
SkyBox::SkyBox(string fileName, string textureName, Modules *modules, Vector3<float> player_pos)
{
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx;

   modelMtx = mat4::Scale(BOXSCALE) * mat4::Translate(player_pos.x, player_pos.y, player_pos.z);
   m_mesh->setShaderType(SHADER_NO_LIGHT);
   m_mesh->setModelMtx(modelMtx);
}
   
   
SkyBox::~SkyBox()
{

}

void SkyBox::tic(uint64_t time, Vector3<float> player_position)
{
   mat4 modelMtx;

   modelMtx = mat4::Scale(BOXSCALE) * 
      mat4::Translate(player_position.x, player_position.y, player_position.z);
   m_mesh->setModelMtx(modelMtx);
}

