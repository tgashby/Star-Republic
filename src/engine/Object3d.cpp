#include "Object3d.h"
#include "Mesh.h"


Object3d::Object3d(string fileName, string textureName, Modules *modules) {
   m_meshList = list<IMesh *>(0);
   Mesh *mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(mesh);
}


Object3d::Object3d() {
   m_meshList = list<IMesh *>(0);
}

Object3d::~Object3d() {
   list<IMesh *>::iterator mesh;
   for (mesh = m_meshList.begin(); mesh != m_meshList.end(); ++mesh) {
      IMesh* temp = *mesh;
      
      delete temp;
   }
}

list<IMesh *>* Object3d::getMeshes() {
   return &m_meshList;
}
