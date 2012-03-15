#ifndef StarRepub_RenderingEngine_h
#define StarRepub_RenderingEngine_h

#define GL_GLEXT_PROTOTYPES
#define PROCESS_TIME 20

#include "Interfaces.h"
#include "RenderingUtils.h"
#include "ResourceLoader.h"
#include "SDL_include.h"
#include "SDL_thread.h"
#include "Mesh.h"
#include <cassert>
#include <map>
#include <cstring>

class RenderingEngine : public IRenderingEngine {
public:
   RenderingEngine(ivec2 screenSize, Modules *modules);
   ~RenderingEngine();
   void setCamera(ICamera *camera);
   void addObject3d(IObject3d *obj);
   void removeObject3d(IObject3d *obj);
   void render(list<IObject3d *> &objects3d, list<IObject3d *> &objects2d);
   void clearScreen();
   void waitForThreads();
   
private:
   void drawMesh(IMesh *mesh, mat4 projection);
   void addLoaded();
   void loadMesh(IMesh *newMesh);
   void unLoadMesh(IMesh *rmvMesh);
   void createShaders();
   void setShaderProgram(SHADER_TYPE type);
   void createFrameBuffers();
   void setFrameBuffer(FRAME_BUFFER_TYPE fb);
   float getPercentHealth();
   ivec2 m_screenSize;
   Modules *m_modules;
   ICamera *m_camera;
   ResourceLoader *m_loader;
   
   Mesh *m_planeMesh;
   vector<IRef*> *m_planeTextures;
   mat4 m_screenMtx;
   
   map<string, MeshRef*> m_meshMap;
   map<string, TextureRef*> m_textureMap;
   
   ShaderProgram *m_curShaderProgram;
   map<SHADER_TYPE, ShaderProgram> m_shaderPrograms;
   
   FrameBuffer *m_curFrameBuffer;
   map<FRAME_BUFFER_TYPE, FrameBuffer> m_frameBuffers;
   
   TTF_Font *m_font;
};

#endif
