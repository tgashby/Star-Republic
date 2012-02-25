#ifndef StarRepub_Mesh_h
#define StarRepub_Mesh_h

#define DEFAULT_SCALE 2

#include "Interfaces.h"

class Mesh: public IMesh {
public:
   Mesh(string meshName, string textureName, Modules *modules);
   Mesh(string meshName, string textureName, float scale, Modules *modules);
   ~Mesh();
   MeshRef getMeshRef();
   void setMeshRef(MeshRef meshRef);
   TextureRef getTextureRef();
   void setTextureRef(TextureRef textureRef);
   void setModelMtx(mat4 modelMtx);
   mat4 getModelMtx();
   void setScale(float scale);
   float getScale();
   vec4 getColor();
   void setColor(vec4 color);
   bool isVisible();
   void setVisible(bool visible);
   MeshBounds getMeshBounds();
   string getMeshName();
   string getTextureName();
   MeshData* getMeshData();
   TextureData* getTextureData();
   SHADER_TYPE getShaderType();
   void setShaderType(SHADER_TYPE type);
   
private:
   MeshRef m_meshRef;
   TextureRef m_textureRef;
   mat4 m_modelMatrix;
   vec4 m_color;
   MeshBounds m_bounds;
   Modules *m_modules;
   bool m_visible;
   float m_scale;
   SHADER_TYPE m_shaderType;
};

#endif
