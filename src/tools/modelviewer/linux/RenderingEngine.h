#ifndef SimpleGame_RenderingEngine_h
#define SimpleGame_RenderingEngine_h

#define GL_GLEXT_PROTOTYPES
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __APPLE__
#include <OPENGL/gl.h>
#include <OPENGL/glu.h>
#endif
#ifdef __unix__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include "Interfaces.h"

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
   RenderingEngine(IResourceManager* resourceManager);
   ~RenderingEngine();
   void setCamera(ICamera *camera);
   void addObject3d(IObject3d *obj);
   void removeObject3d(IObject3d *obj);
   void render(list<IObject3d *> &objects);
private:
   GLuint buildShader(const char* source, GLenum shaderType) const;
   GLuint buildProgram(const char* vShader, const char* fShader) const;
   void loadMesh(IMesh *newMesh);
   void unLoadMesh(IMesh *rmvMesh);
   list<MeshRef>::iterator findMeshRef(MeshRef mesh);
   list<TextureRef>::iterator findTextureRef(TextureRef texture);
   IResourceManager *m_resourceManager;
   UniformHandles m_uniforms;
   AttributeHandles m_attributes;
   //GLuint m_testTexture;
   ICamera *m_camera;
   //list<IObject3d *> m_objectList;
   list<MeshRef> m_meshList;
   list<TextureRef> m_textureList;
};

#endif