#include "HealthBar.h"

HealthBar::HealthBar(Modules *modules) : Object2d() {
   ivec2 scale = ivec2(256, 32);
   ivec2 loc = ivec2(-390, 260);

   m_mainMesh = new Mesh(PLANE_MESH_FILE, HEALTH_BAR_TEXTURE2, modules);
   m_mainMesh->setScale3v(vec3(scale.x, scale.y, 1));
   m_mainMesh->setModelMtx(mat4::Translate(loc.x, loc.y, 0));
   m_mainMesh->setTextureMtx(mat4::Scale(1, -1, 1));
   m_mainMesh->setShaderType(SHADER_NO_LIGHT);
   m_meshList.push_back(m_mainMesh);

   m_mainMesh = new Mesh(PLANE_MESH_FILE, HEALTH_BAR_TEXTURE1, modules);
   m_mainMesh->setScale3v(vec3(scale.x, scale.y, 1));
   m_mainMesh->setModelMtx(mat4::Translate(loc.x, loc.y, 0));
   m_mainMesh->setTextureMtx(mat4::Scale(1, -1, 1));
   m_mainMesh->setShaderType(SHADER_NO_LIGHT);
   m_meshList.push_back(m_mainMesh);
}

HealthBar::~HealthBar() {
   
}

void HealthBar::updateBar(float healthPercentage) {
   ivec2 scale = ivec2(256, 32);
   
   m_mainMesh->setScale3v(vec3(scale.x * healthPercentage, scale.y, 1));
}
