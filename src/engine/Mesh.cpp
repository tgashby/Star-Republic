#include "Mesh.h"

Mesh::Mesh(string meshName, string textureName, Modules *modules) {
   m_meshName = meshName;
   m_textureName = textureName;
   m_meshRef = NULL;
   m_textureRef = NULL;
   
   m_modules = modules;
   m_modelMatrix = mat4::Identity();
   m_color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
   m_visible = true;
   m_scale = DEFAULT_SCALE;
   m_shaderType = SHADER_VERTEX_LIGHT;
}

Mesh::Mesh(string meshName, string textureName, float scale, Modules *modules) {
   m_meshName = meshName;
   m_textureName = textureName;
   m_meshRef = NULL;
   m_textureRef = NULL;
   
   m_modules = modules;
   m_modelMatrix = mat4::Identity();
   m_color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
   m_visible = true;
   m_scale = scale;
   m_shaderType = SHADER_VERTEX_LIGHT;
}

Mesh::~Mesh() {
   
}

MeshRef* Mesh::getMeshRef() {
   return m_meshRef;
}

void Mesh::setMeshRef(MeshRef *meshRef) {
   m_meshRef = meshRef;
}

TextureRef* Mesh::getTextureRef() {
   return m_textureRef;
}

void Mesh::setTextureRef(TextureRef *textureRef) {
   m_textureRef = textureRef;
}

void Mesh::setModelMtx(mat4 modelMtx) {
   m_modelMatrix = modelMtx;
}

mat4 Mesh::getModelMtx() {
   return m_modelMatrix;
}

void Mesh::setScale(float scale) {
   m_scale = scale;
}

float Mesh::getScale() {
   return m_scale;
}

vec4 Mesh::getColor() {
   return m_color;
}

void Mesh::setColor(vec4 color) {
   m_color = color;
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

string Mesh::getMeshName() {
   return m_meshName;
}

string Mesh::getTextureName() {
   return m_textureName;
}

MeshData* Mesh::getMeshData() {
   return m_modules->resourceManager->readMeshData(m_meshName, LOAD_NORMAL_VERTEX, 1.0);
}

TextureData* Mesh::getTextureData() {
   return m_modules->resourceManager->loadBMPImage(m_textureName);
}

SHADER_TYPE Mesh::getShaderType() {
   return m_shaderType;
}

void Mesh::setShaderType(SHADER_TYPE type) {
   m_shaderType = type;
}