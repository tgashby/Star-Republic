#include "Mesh.h"

Mesh::Mesh(string meshName, string textureName, Modules modules) {
   m_meshName = meshName;
   m_textureName = textureName;
   m_meshRef = MeshRef();
   m_modelMatrix = mat4::Identity();
   m_color = vec4(0.8, 0.8, 0.8, 1.0);
}

Mesh::~Mesh() {
   
}

MeshRef* Mesh::getMeshRef() {
   return NULL;
}

TextureRef* Mesh::getTextureRef() {
   return NULL;
}

void Mesh::setModelMatrix(mat4 modelMtx) {
   m_modelMatrix = modelMtx;
}

mat4 Mesh::getModelMatrix() {
   return m_modelMatrix;
}

vec4 Mesh::getColor() {
   return m_color;
}

MeshBounds Mesh::getMeshBounds() {
   return MeshBounds();
}

string Mesh::getMeshName() {
   return m_meshName;
}

string Mesh::getTextureName() {
   return m_textureName;
}

MeshData* Mesh::getMeshData() {
   return NULL;
}

TextureData* Mesh::getTextureData() {
   return NULL;
}