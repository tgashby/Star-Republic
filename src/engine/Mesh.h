#ifndef StarRepub_Mesh_h
#define StarRepub_Mesh_h

#define DEFAULT_SCALE 2

#include "Interfaces.h"

class Mesh: public IMesh {
public:
   Mesh(string meshName, string textureName, Modules *modules);
   Mesh(string meshName, string textureName, float scale, Modules *modules);
   ~Mesh();
   IRef* getMeshRef();
   void setMeshRef(IRef *meshRef);
   vector<IRef*>* getTextureRefs();
   int getNumTextureRefs();
   IRef* getTextureRef(int index);
   void setModelMtx(mat4 modelMtx);
   mat4 getModelMtx();
   void setScale(float scale);
   void setScale3v(vec3 scale);
   mat4 getTextureMtx();
   void setTextureMtx(mat4 textureMtx);
   vec3 getScale();
   vec4 getColor();
   void setColor(vec4 color);
   bool checkLoaded();
   bool isVisible();
   void setVisible(bool visible);
   MeshBounds getMeshBounds();
   SHADER_TYPE getShaderType();
   void setShaderType(SHADER_TYPE type);
   
private:
   IRef *m_meshRef;
   vector<IRef*> m_textureRefs;
   mat4 m_modelMatrix;
   mat4 m_textureMatrix;
   vec4 m_color;
   MeshBounds m_bounds;
   Modules *m_modules;
   bool m_visible;
   vec3 m_scale;
   SHADER_TYPE m_shaderType;
};

#endif
