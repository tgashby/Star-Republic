#include "Menu.h"

Menu::Menu(string fileName, string textureName, Modules *modules)
{
  m_mesh = new Mesh(fileName, textureName, modules);
  m_meshList.push_back(m_mesh);

  modelMtx = mat4::Scale(1) * mat4::Translate(0,0,-20);
  m_mesh->setModelMtx(modelMtx);

  m_mesh->setVisible(true);

}

Menu::~Menu()
{
   delete m_mesh;
}


