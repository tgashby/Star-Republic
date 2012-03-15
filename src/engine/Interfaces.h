#ifndef StarRepub_Interfaces_h
#define StarRepub_Interfaces_h

/*
 this file should contain includes, namespaces, structs, and abstract classes
 shared by all objects in the program
 */

#include "SDL_include.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "WorldData.h"
#include <vector>
#include <list>
#include <string>
#include <iostream>

// Used for unit testing
#define GAME_DEBUG

// Uncomment this line for production
// #undef GAME_DEBUG


using namespace std;


// Used by the ResourceManager to determine how to load meshes.
enum LOAD_NORMAL_TYPE {
   LOAD_NORMAL_VERTEX,
   LOAD_NORMAL_FACE,
};

enum SHADER_TYPE {
   SHADER_VERTEX_LIGHT,
   SHADER_NO_LIGHT,
   SHADER_BLOOM,
   SHADER_BLOOM_CULL,
   SHADER_COMBINE,
};

struct MeshBounds {
   vec3 mean;
   vec3 min;
   vec3 max;
};

// Extended by the Rendering Engine for referencing Meshes and Textures
struct IRef {
   IRef() {
      fileName = "";
      loaded = false;
      loading = false;
   }
   IRef(string name) {
      fileName = name;
      loaded = false;
      loading = false;
   }
   string fileName;
   bool loaded;
   bool loading;
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

// holds date for textures
struct TextureData {
   void* pixels;
   void* data;
   ivec2 size;
};

// An abstract class for the 
struct IMesh {
   virtual ~IMesh() {}
   virtual IRef* getMeshRef() = 0;
   virtual void setMeshRef(IRef *meshRef) = 0;
   virtual vector<IRef*>* getTextureRefs() = 0;
   virtual int getNumTextureRefs() = 0;
   virtual IRef* getTextureRef(int index) = 0;
   virtual void setModelMtx(mat4 modelMtx) = 0;
   virtual mat4 getModelMtx() = 0;
   virtual void setScale(float scale) = 0;
   virtual void setScale3v(vec3 scale) = 0;
   virtual mat4 getTextureMtx() = 0;
   virtual void setTextureMtx(mat4 textureMtx) = 0;
   virtual vec3 getScale() = 0;
   virtual vec4 getColor() = 0;
   virtual void setColor(vec4 color) = 0;
   virtual bool checkLoaded() = 0;
   virtual bool isVisible() = 0;
   virtual void setVisible(bool visible) = 0;
   virtual MeshBounds getMeshBounds() = 0;
   virtual SHADER_TYPE getShaderType() = 0;
   virtual void setShaderType(SHADER_TYPE type) = 0;
};

// Accessed by the RenderingEngine for a combined projection and view matrix.
struct ICamera {
   virtual ~ICamera() {}
   virtual mat4 getProjectionViewMtx() = 0;
};

// Each object needs to provide the RenderingEngine a list of meshes.
struct IObject3d {
   virtual ~IObject3d() {}
   virtual list<IMesh *>* getMeshes() = 0;
};

// Sound
const int numSoundEvents = 5; 
enum SoundEvent {
   Explosion = 0,
   PlayerGun,
   PlayerShotgun,
   PlayerHit,
   PlayerMissile,
};

const int numBackgroundSounds = 2;
enum BackgroundSound {
   Boost = 0,
   Idle
};

class ISoundManager {
public:
   virtual ~ISoundManager(){}; 
   virtual void playSound(SoundEvent event) = 0;

   virtual void playBackgroundSound(BackgroundSound sound) = 0;
   virtual void stopBackgroundSound(BackgroundSound sound) = 0; 
   virtual void startMusic() = 0;
   virtual void stopMusic() = 0;
   virtual void tic(uint64_t dt) = 0;
};

const int INVALID_CHANNEL = -2;
class ISound 
{
public:
   virtual ~ISound() {}
   virtual void play(int loops) = 0;
   virtual void pause() = 0;
   virtual void resume() = 0;
protected:
   int m_channel;
   Mix_Chunk* m_chunk;
   Mix_Music* m_music;
};

// The main class that runs the game.
struct IGameEngine {
   virtual ~IGameEngine() {}
   virtual void tic(uint64_t td) = 0;
   virtual void render() = 0;
   virtual bool handleEvents() = 0;
   virtual bool handleKeyUp(SDLKey key) = 0;
   virtual void handleMouseMotion(Uint16 x, Uint16 y) = 0;
   virtual ICamera& getCamera() = 0;
};


// All OpenGL calls should be implemented in the RenderingEngine.
struct IRenderingEngine {
   virtual ~IRenderingEngine() {}
   virtual void render(list<IObject3d *> &objects3d, list<IObject3d *> &objects2d) = 0;
   virtual void setCamera(ICamera *camera) = 0;
   virtual void addObject3d(IObject3d *obj) = 0;
   virtual void removeObject3d(IObject3d *obj) = 0;
   virtual void drawMesh(IMesh *mesh, mat4 projection) = 0;
   virtual void drawText(string text, ivec2 loc, ivec2 size) = 0;
   virtual void clearScreen() = 0;
   virtual void waitForThreads() = 0;
};

// Any resurces from the file system should be accessed
// through this class.
struct IResourceManager {
   virtual ~IResourceManager() {}
   virtual MeshData *readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale) = 0;
   virtual TextureData *loadBMPImage(string fileName) = 0;
   //virtual ImageData *loadPngImage(string fileName) = 0;
   virtual WorldData *readWorldData(string fileName) = 0;
   virtual string readShader(string name) = 0;
};


/**
 * Modules - Keeps track of the various major components of the program
 * This should be created in Main.cpp to be passed as a reference to any
 * object needing to accesss these instances.
 */
struct Modules {
   IGameEngine *gameEngine;
   IRenderingEngine *renderingEngine;
   IResourceManager *resourceManager;
   ISoundManager *soundManager;
   
   ~Modules()
   {
      delete gameEngine;
      delete renderingEngine;
      delete resourceManager;
      delete soundManager;
   }
};

#endif
