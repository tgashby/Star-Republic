#ifndef StarRepub_RenderingEngine_h
#define StarRepub_RenderingEngine_h

#define GL_GLEXT_PROTOTYPES

#include "SDL_include.h"

#include "Interfaces.h"
#include <cassert>
#include <map>

#define VERTEX_STRIDE 11
#define NORMAL_OFFSET 3
#define TEXTURE_OFFSET 9

struct UniformHandles {
   GLuint modelview;
   GLuint projection;
   GLuint normalMatrix;
   GLuint lightPosition;
   GLuint ambientMaterial;
   GLuint specularMaterial;
   GLuint shininess;
   GLuint sampler;
};

struct AttributeHandles {
   GLuint position;
   GLuint normal;
   GLuint diffuseMaterial;
   GLuint textureCoord;
};

struct ShaderProgram {
   SHADER_TYPE type;
   GLuint program;
   GLuint vertexShader;
   GLuint fragmentShader;
   UniformHandles uniforms;
   AttributeHandles attributes;
};

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
   void createShaders();
   GLuint buildShader(const char* source, GLenum shaderType) const;
   ShaderProgram buildProgram(const char* vShader, const char* fShader, SHADER_TYPE type);
   void useProgram(SHADER_TYPE type);
   void loadMesh(IMesh *newMesh);
   void unLoadMesh(IMesh *rmvMesh);
   ivec2 m_screenSize;
   Modules *m_modules;
   ICamera *m_camera;
   //list<MeshRef> m_meshList;
   //list<TextureRef> m_textureList;
   map<string, MeshRef> m_meshMap;
   map<string, TextureRef> m_textureMap;
   GLuint m_planeVert;
   GLuint m_planeInt;
   TTF_Font *font;
   map<SHADER_TYPE, ShaderProgram> m_shaderPrograms;
   //ShaderProgram m_vertexLightShader;
   //ShaderProgram m_noLightShader;
   ShaderProgram *m_curShaderProgram;
};

#endif
