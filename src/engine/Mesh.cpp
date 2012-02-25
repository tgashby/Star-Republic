#include "Mesh.h"

Mesh::Mesh(string meshName, string textureName, Modules *modules) {
   m_meshRef = MeshRef();
   m_meshRef.name = meshName;
   m_modules = modules;
   
   m_textureRef = TextureRef();
   m_textureRef.name = textureName;
   
   m_modelMatrix = mat4::Identity();
   m_color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
   m_visible = true;
   m_scale = DEFAULT_SCALE;
   m_shaderType = SHADER_VERTEX_LIGHT;
}

Mesh::Mesh(string meshName, string textureName, float scale, Modules *modules) {
   m_meshRef = MeshRef();
   m_meshRef.name = meshName;
   m_modules = modules;
   
   m_textureRef = TextureRef();
   m_textureRef.name = textureName;
   
   m_modelMatrix = mat4::Identity();
   m_color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
   m_visible = true;
   m_scale = scale;
}

Mesh::~Mesh() {
   
}

MeshRef Mesh::getMeshRef() {
   return m_meshRef;
}

void Mesh::setMeshRef(MeshRef meshRef) {
   m_meshRef = meshRef;
}

TextureRef Mesh::getTextureRef() {
   return m_textureRef;
}

void Mesh::setTextureRef(TextureRef textureRef) {
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
   return m_meshRef.bounds;
}

string Mesh::getMeshName() {
   return m_meshRef.name;
}

string Mesh::getTextureName() {
   return m_textureRef.name;
}

MeshData* Mesh::getMeshData() {
   return m_modules->resourceManager->readMeshData(m_meshRef.name,
                                                   LOAD_NORMAL_VERTEX,
                                                   1.0);
}

TextureData* Mesh::getTextureData() {
   return m_modules->resourceManager->loadBMPImage(m_textureRef.name);
}

SHADER_TYPE Mesh::getShaderType() {
   return m_shaderType;
}

void Mesh::setShaderType(SHADER_TYPE type) {
   m_shaderType = type;
}