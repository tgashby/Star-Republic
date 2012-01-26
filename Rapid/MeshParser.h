#ifndef ModelViewer_MeshParser_h
#define ModelViewer_MeshParser_h

//#include "Interfaces.h"
#include "Util/Vector.hpp"
#include <vector>
#include <string>

enum LOAD_NORMAL_TYPE {
   LOAD_NORMAL_VERTEX,
   LOAD_NORMAL_FACE,
};

// Data for creating VBOs
struct MeshData {
   int vertexCount;
   float* vertices;
   int indexCount;
   unsigned short* indices;
   vec3 mean, min, max;
};

class Face {
public:
   void setFaceNormal(std::vector<vec3 *> verts) {
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
      //cout << "(" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
   }
   ivec3 vertices;
   ivec3 normals;
   ivec3 tangents;
   ivec3 textures;
   //used for flat normal loading
   vec3 normal;
};

MeshData* loadMesh(std::string fileName, LOAD_NORMAL_TYPE type, float scale);
float parseFloat(std::string line);
vec3* parseVertex(std::string line);
vec3* parseNormal(std::string line);
vec3* parseTangent(std::string line);
vec2* parseTexture(std::string line);
Face* parseFace(std::string line);
void combineVertex(float *vertices, vec3 *vertex, vec3 *normal, vec3 *tangent, vec2 *texture);

#endif
