#ifndef StarRepub_ResourceManager_h
#define StarRepub_ResourceManager_h

#include "Interfaces.h"
#include "Sound.h"
#include "SDL_include.h"

class ResourceManager : public IResourceManager {
public:
   ResourceManager();
   MeshData* readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale);
   TextureData* loadBMPImage(string fileName);
   //ImageData* loadPngImage(string fileName);
   ~ResourceManager();
private:
   SDL_Surface *m_image;
};


class Face {
public:
   void setFaceNormal(vector<vec3 *> verts) {
      vec3 *p1, *p2 ,*p3, v1, v2;
      p1 = verts[vertices.x];
      p2 = verts[vertices.y];
      p3 = verts[vertices.z];
      v1 = *p1 - *p2;
      //v1.Normalize();
      v2 = *p1 - *p3;
      //v2.Normalize();
      normal = v2.Cross(v1);
      normal.Normalize();
      cout << "(" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
   }
   ivec3 vertices;
   ivec3 normals;
   ivec3 tangents;
   ivec3 textures;
   //used for flat normal loading
   vec3 normal;
};

MeshData* loadMesh(string fileName, LOAD_NORMAL_TYPE type, float scale);
float parseFloat(string line);
vec3* parseVertex(string line);
vec3* parseNormal(string line);
vec3* parseTangent(string line);
vec2* parseTexture(string line);
Face* parseFace(string line);
void combineVertex(float *vertices, vec3 *vertex, vec3 *normal, vec3 *tangent, vec2 *texture);

void initSound();
Sound* loadSound(string fileName);
Sound* loadMusic(string fileName);

#endif
