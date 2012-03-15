#include "Object2d.h"
#include "Mesh.h"


Object2d::Object2d(string textureName, ivec2 loc, ivec2 size, Modules *modules) {
   m_meshList = list<IMesh *>(0);
   Mesh *mesh = new Mesh(PLANE_MESH_FILE, textureName, modules);
   mesh->setScale3v(vec3(size.x, size.y, 1));
   mesh->setModelMtx(mat4::Translate(loc.x, loc.y, 0));
   mesh->setTextureMtx(mat4::Scale(1, -1, 1));
   mesh->setShaderType(SHADER_NO_LIGHT);
   m_meshList.push_back(mesh);
}

Object2d::Object2d() {
   m_meshList = list<IMesh *>(0);
}

Object2d::~Object2d() {
   list<IMesh *>::iterator mesh;
   for (mesh = m_meshList.begin(); mesh != m_meshList.end(); ++mesh) {
      IMesh* temp = *mesh;
      
      delete temp;
   }
}

list<IMesh *>* Object2d::getMeshes() {
   return &m_meshList;
}

void Object2d::setVisible(bool vis) {
   list<IMesh *>::iterator mesh;
   for (mesh = m_meshList.begin(); mesh != m_meshList.end(); ++mesh) {
      IMesh* temp = *mesh;
      
      temp->setVisible(vis);
   }
}

bool Object2d::viewCull(vector<vec4> *planes) {
   return true;
}