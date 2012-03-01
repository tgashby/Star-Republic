#ifndef StarRepub_RenderingEngine_h
#define StarRepub_RenderingEngine_h

#define GL_GLEXT_PROTOTYPES
#define FILTER_COUNT 4
#define PROCESS_TIME 20

#include "Interfaces.h"
#include "RenderingUtils.h"
#include "SDL_include.h"
#include "SDL_thread.h"
#include "Mesh.h"
#include <cassert>
#include <map>

class RenderingEngine : public IRenderingEngine {
public:
   RenderingEngine(ivec2 screenSize, Modules *modules);
   ~RenderingEngine();
   void setCamera(ICamera *camera);
   void addObject3d(IObject3d *obj);
   void removeObject3d(IObject3d *obj);
   void render(list<IObject3d *> &objects);
   void drawText(string text, ivec2 loc, ivec2 size);
   void clearScreen();
private:
   void drawMesh(IMesh *mesh, mat4 projection);
   void processJobs();
   void addMesh(MeshData *meshData, MeshRef *meshRef);
   void addTexture(TextureData *textureData, TextureRef *textureRef);
   void createShaders();
   GLuint buildShader(const char* source, GLenum shaderType) const;
   ShaderProgram buildProgram(const char* vShader, const char* fShader, SHADER_TYPE type);
   void useProgram(SHADER_TYPE type);
   void loadMesh(IMesh *newMesh);
   void unLoadMesh(IMesh *rmvMesh);
   
   Surface pass0;
   Surface pass1;
   Surface reduce0;
   Surface reduce1;
   Surface reduce2;
   Surface reduce3;
   
   Mesh *m_planeMesh;
   vector<IRef*> *m_planeTextures;
   
   ivec2 m_screenSize;
   Modules *m_modules;
   ICamera *m_camera;
   map<string, MeshRef*> m_meshMap;
   map<string, TextureRef*> m_textureMap;
   GLuint m_planeVert;
   GLuint m_planeInt;
   TTF_Font *font;
   map<SHADER_TYPE, ShaderProgram> m_shaderPrograms;
   ShaderProgram *m_curShaderProgram;
   LoadingJobs *m_jobs;
};

#endif
