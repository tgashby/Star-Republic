#include "Mesh.h"

Mesh::Mesh(string meshName, string textureName, Modules *modules) {
   m_meshRef = new IRef(meshName);
   m_textureRefs.push_back(new IRef(textureName));
   
   m_modules = modules;
   m_modelMatrix = mat4::Identity();
   m_textureMatrix = mat4::Identity();
   m_color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
   m_visible = true;
   m_scale = vec3(DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE);
   m_shaderType = SHADER_VERTEX_LIGHT;
}

Mesh::Mesh(string meshName, string textureName, float scale, Modules *modules) {
   m_meshRef = new IRef(meshName);
   m_textureRefs.push_back(new IRef(textureName));
   
   m_modules = modules;
   m_modelMatrix = mat4::Identity();
   m_color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
   m_visible = true;
   m_scale = vec3(scale, scale, scale);
   m_shaderType = SHADER_VERTEX_LIGHT;
}

Mesh::~Mesh() {
   
}

IRef* Mesh::getMeshRef() {
   return m_meshRef;
}

void Mesh::setMeshRef(IRef *meshRef) {
   m_meshRef = meshRef;
}


vector<IRef*>* Mesh::getTextureRefs() {
   return &m_textureRefs;
}

int Mesh::getNumTextureRefs() {
   return (int) m_textureRefs.size();
}

IRef* Mesh::getTextureRef(int index) {
   return m_textureRefs.at(index);
}

void Mesh::setModelMtx(mat4 modelMtx) {
   m_modelMatrix = modelMtx;
}

mat4 Mesh::getModelMtx() {
   return m_modelMatrix;
}

void Mesh::setScale(float scale) {
   m_scale = vec3(scale, scale, scale);
}

void Mesh::setScale3v(vec3 scale) {
   m_scale = scale;
}

vec3 Mesh::getScale() {
   return m_scale;
}

mat4 Mesh::getTextureMtx() {
   return m_textureMatrix;
}

void Mesh::setTextureMtx(mat4 textureMtx) {
   m_textureMatrix = textureMtx;
}

vec4 Mesh::getColor() {
   return m_color;
}

void Mesh::setColor(vec4 color) {
   m_color = color;
}

bool Mesh::checkLoaded() {
   bool loaded = m_meshRef->loaded;
   vector<IRef*>::iterator refIter = m_textureRefs.begin();
   while (refIter != m_textureRefs.end()) {
      loaded = loaded && (*refIter)->loaded;
      ++refIter;
   }
   return loaded;
}

bool Mesh::isVisible() {
   return m_visible;
}

void Mesh::setVisible(bool visible) {
   m_visible = visible;
}

MeshBounds Mesh::getMeshBounds() {
   if (m_meshRef == NULL) {
      return MeshBounds();
   }
   return m_meshRef->bounds;
}

SHADER_TYPE Mesh::getShaderType() {
   return m_shaderType;
}

void Mesh::setShaderType(SHADER_TYPE type) {
   m_shaderType = type;
}