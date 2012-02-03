#ifndef StarRepub_Interfaces_h
#define StarRepub_Interfaces_h

/*
 this file should contain includes, namespaces, structs, and abstract classes
 shared by all objects in the program
 */

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <vector>
#include <list>
#include <string>
#include <iostream>

using namespace std;
using std::vector;
using std::list;
using std::find;
using std::string;


// Used by the ResourceManager to determine how to load meshes.
enum LOAD_NORMAL_TYPE {
   LOAD_NORMAL_VERTEX,
   LOAD_NORMAL_FACE,
};

struct MeshBounds {
   vec3 mean;
   vec3 min;
   vec3 max;
};

// Used by the RenderingEngine to keep track of mesh VBOs.
struct MeshRef {
   MeshRef() {
      name = "";
      vertexBuffer = 0;
      indexBuffer = 0;
      indexCount = 0;
      count = 0;
      bounds = MeshBounds();
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
      bounds = other.bounds;
   }
   // Used to find duplicate VBOs.
   int operator==(const MeshRef &rhs) {
      if (name == rhs.name) return 1;
      return 0;
   }
   string name;
   unsigned int vertexBuffer;
   unsigned int indexBuffer;
   int indexCount;
   int count;
   MeshBounds bounds;
};


// Holds data for creating Mesh VBOs
struct MeshData {
   int vertexCount;
   float* vertices;
   int indexCount;
   unsigned short* indices;
   MeshBounds bounds;
};

// Used by the RenderingEngine to keep track of mesh VBOs.
struct TextureRef {
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

// holds date for textures
struct TextureData {
   void* pixels;
   ivec2 size;
};

// An abstract class for the 
struct IMesh {
   virtual MeshRef getMeshRef() = 0;
   virtual void setMeshRef(MeshRef meshRef) = 0;
   virtual TextureRef getTextureRef() = 0;
   virtual void setTextureRef(TextureRef textureRef) = 0;
   virtual void setModelMtx(mat4 modelMtx) = 0;
   virtual mat4 getModelMtx() = 0;
   virtual vec4 getColor() = 0;
   virtual void setColor(vec4 color) = 0;
   virtual bool isVisible() = 0;
   virtual void setVisible(bool visible) = 0;
   virtual MeshBounds getMeshBounds() = 0;
   
   // File name and Mesh type
   // Name used as a key by Rendering Engine for finding duplicate meshes.
   virtual string getMeshName() = 0;
   virtual string getTextureName() = 0;
   
   virtual MeshData* getMeshData() = 0;
   virtual TextureData* getTextureData() = 0;
};


// Accessed by the RenderingEngine for a combined projection and view matrix.
struct ICamera {
   virtual mat4 getProjectionViewMtx() = 0;
};


// Each object needs to provide the RenderingEngine a list of meshes.
struct IObject3d {
   virtual list<IMesh *>* getMeshes() = 0;
};


// The main class that runs the game.
struct IGameEngine {
   virtual void tic(unsigned int td) = 0;
   virtual void render() = 0;
};


// All OpenGL calls should be implemented in the RenderingEngine.
struct IRenderingEngine {
   virtual void render(list<IObject3d *> &objects) = 0;
   virtual void setCamera(ICamera *camera) = 0;
   virtual void addObject3d(IObject3d *obj) = 0;
   virtual void removeObject3d(IObject3d *obj) = 0;
};


// Any resurces from the file system should be accessed
// through this class.
struct IResourceManager {
   virtual MeshData* readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale) = 0;
   virtual TextureData* loadBMPImage(string fileName) = 0;
   //virtual ImageData* loadPngImage(string fileName) = 0;
};


// This should be created in Main.cpp to be passed as a reference to any
// object needing to accesss these instances.
struct Modules {
   IGameEngine *gameEngine;
   IRenderingEngine *renderingEngine;
   IResourceManager *resourceManager;
};

#endif
