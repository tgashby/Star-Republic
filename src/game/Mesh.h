#ifndef StarRepub_Mesh_h
#define StarRepub_Mesh_h

#include "../Interfaces.h"

class Mesh: public IMesh {
public:
   Mesh(string meshName, string textureName, Modules modules);
   ~Mesh();
   MeshRef* getMeshRef();
   TextureRef* getTextureRef();
   void setModelMatrix(mat4 modelMtx);
   mat4 getModelMatrix();
   vec4 getColor();
   MeshBounds getMeshBounds();
   string getMeshName();
   string getTextureName();
   MeshData* getMeshData();
   TextureData* getTextureData();
private:
   string m_meshName;
   string m_textureName;
   MeshRef m_meshRef;
   TextureRef m_textureRef;
   mat4 m_modelMatrix;
   vec4 m_color;
   MeshBounds m_bounds;
   Modules m_modules;
};

#endif
