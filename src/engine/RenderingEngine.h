#ifndef StarRepub_RenderingEngine_h
#define StarRepub_RenderingEngine_h

#define GL_GLEXT_PROTOTYPES

#include "SDL_include.h"

#include "Interfaces.h"
#include <cassert>

#define VERTEX_STRIDE 11
#define NORMAL_OFFSET 3
#define TEXTURE_OFFSET 9

struct UniformHandles {
   GLuint modelview;
   GLuint projection;
   GLuint normalMatrix;
   GLuint lightPosition;
   GLint ambientMaterial;
   GLint specularMaterial;
   GLint shininess;
   GLint sampler;
};

struct AttributeHandles {
   GLint position;
   GLint normal;
   GLint diffuseMaterial;
   GLint textureCoord;
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
private:
   GLuint buildShader(const char* source, GLenum shaderType) const;
   GLuint buildProgram(const char* vShader, const char* fShader) const;
   void loadMesh(IMesh *newMesh);
   void unLoadMesh(IMesh *rmvMesh);
   ivec2 m_screenSize;
   Modules *m_modules;
   UniformHandles m_uniforms;
   AttributeHandles m_attributes;
   ICamera *m_camera;
   list<MeshRef> m_meshList;
   list<TextureRef> m_textureList;
   GLuint m_planeVert;
   GLuint m_planeInt;
   TTF_Font *font;
};

#endif
