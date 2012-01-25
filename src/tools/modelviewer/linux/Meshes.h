#ifndef SimpleGame_Meshes_h
#define SimpleGame_Meshes_h

#include "Interfaces.h"

class Mesh: public IMesh {
public:
   Mesh(string meshName, string textureName);
   ~Mesh();
   string getMeshName();
   string getTextureName();
   MeshData* getMeshData();
   ImageData* getImageData();
private:
   string m_meshName;
   string m_textureName;
};

#endif
