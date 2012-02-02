#include "Object3d.h"
#include "Meshes.h"
#include <iostream>

Object3d::Object3d() {
   m_meshList = list<IMesh *>(0);
}

Object3d::Object3d(string fileName, string textureName) {
   m_meshList = list<IMesh *>(0);
   Mesh *mesh = new Mesh(fileName, textureName);
   m_meshList.push_back(mesh);
}

Object3d::~Object3d() {
   list<IMesh *>::iterator mesh;
   for (mesh = m_meshList.begin(); mesh != m_meshList.end(); ++mesh) {
      delete *mesh;
   }
}

list<IMesh *>* Object3d::getMeshes() {
   return &m_meshList;
}
