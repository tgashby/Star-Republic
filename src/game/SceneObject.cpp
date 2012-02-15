#include "SceneObject.h"
#include "Mesh.h"

SceneObject::SceneObject(string fileName, string textureName, vec3 pos, vec3 fwd, vec3 up, Modules *modules) : Object3d() {
   Mesh *newMesh = new Mesh(fileName, textureName, 1.0, modules);
   newMesh->setModelMtx(mat4::Magic(fwd, up, pos));
   m_meshList.push_back(newMesh);
}