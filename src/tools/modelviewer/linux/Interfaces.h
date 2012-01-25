#ifndef SimpleGame_Interfaces_h
#define SimpleGame_Interfaces_h

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include <vector>
#include <list>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;
using std::vector;
using std::list;
using std::find;
using std::string;


enum LOAD_NORMAL_TYPE {
   LOAD_NORMAL_VERTEX,
   LOAD_NORMAL_FACE,
};

// RenderingEngine uses this for keeping track of VBOs
class MeshRef {
public:
   MeshRef() {
      name = "";
      vertexBuffer = 0;
      indexBuffer = 0;
      indexCount = 0;
      count = 0;
   }
   MeshRef(string n, unsigned int vb, unsigned int ib, int idxCount) {
      name = n;
      vertexBuffer = vb;
      indexBuffer = ib;
      indexCount = idxCount;
      count = 1;
   }
   MeshRef(const MeshRef &other) {
      name = other.name;
      vertexBuffer = other.vertexBuffer;
      indexBuffer = other.indexBuffer;
      indexCount = other.indexCount;
      count = other.count;
   }
   int operator==(const MeshRef &rhs) {
      if (name == rhs.name) return 1;
      return 0;
   }
   string name;
   unsigned int vertexBuffer;
   unsigned int indexBuffer;
   int indexCount;
   int count;
};

// Data for creating VBOs
struct MeshData {
   int vertexCount;
   float* vertices;
   int indexCount;
   unsigned short* indices;
   vec3 mean, min, max;
};

class TextureRef {
public:
   TextureRef() {
      name = "";
      count = 0;
   }
   TextureRef(string n, unsigned int tb) {
      name = n;
      textureBuffer = tb;
      count = 1;
   }
   TextureRef(const TextureRef &other) {
      name = other.name;
      textureBuffer = other.textureBuffer;
      count = other.count;
   }
   int operator==(const TextureRef &rhs) {
      if (name == rhs.name) return 1;
      return 0;
   }
   string name;
   unsigned int textureBuffer;
   int count;
};

struct ImageData {
   void* pixels;
   ivec2 size;
};

// Stores information for a single mesh.
struct IMesh {
   MeshRef meshRef;   // References used by render engine
   TextureRef textureRef;
   mat4 meshMtx;        // Matrix used by render engine
   vec3 mean, min, max; // Bounding box.
   vec4 color;
   float size;        // the size to load the mesh as.
   LOAD_NORMAL_TYPE normalType;
   
   // File name and Mesh type
   // Name used as a key by Rendering Engine for finding duplicate meshes.
   virtual string getMeshName() = 0;
   virtual string getTextureName() = 0;
   
   // Used for procedural meshes. Meshes on file should be 
   virtual MeshData* getMeshData() = 0;
   virtual ImageData* getImageData() = 0;
   virtual ~IMesh() {}
};

struct ICamera {
   vec3 fwd;
   vec3 eye;
   vec3 ref;
   vec3 up;
};

struct IObject3d {
   virtual list<IMesh *>* getMeshes() = 0;
};

// Acts as an interface to SDL for the game.
struct IApplicationEngine {
   virtual void render() = 0;
   virtual ~IApplicationEngine() {}
};

// All OpenGL calls should be implemented here.
struct IRenderingEngine {
   virtual void render(list<IObject3d *> &objects) = 0;
   virtual void setCamera(ICamera *camera) = 0;
   virtual void addObject3d(IObject3d *obj) = 0;
   virtual void removeObject3d(IObject3d *obj) = 0;
   virtual ~IRenderingEngine() {}
};

// Any resurces from the file system should be accessed
// through this class.
struct IResourceManager {
   virtual ~IResourceManager() {}
   virtual ImageData* loadBMPImage(string fileName) = 0;
   //virtual ImageData* loadPngImage(string fileName) = 0;
   virtual void unLoadImage() = 0;
};

#endif
