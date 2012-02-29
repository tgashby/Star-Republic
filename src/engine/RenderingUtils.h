#ifndef StarRepub_RenderingUtils_h
#define StarRepub_RenderingUtils_h

#define GL_GLEXT_PROTOTYPES

#include "Interfaces.h"
#include "SDL_include.h"
#include "SDL_thread.h"

#define VERTEX_STRIDE 11
#define NORMAL_OFFSET 3
#define TEXTURE_OFFSET 9

struct ViewPort {
   ivec2 loc;
   ivec2 size;
};

struct Surface {
   ivec2 size;
   ViewPort viewport;
   vec4 clearColor;
   mat4 modelView;
   mat4 projection;
   GLuint texture;
   GLuint depth;
   GLuint fbo;
};

struct UniformHandles {
   GLuint modelview;
   GLuint projection;
   GLuint normalMatrix;
   GLuint lightPosition;
   GLuint ambientMaterial;
   GLuint specularMaterial;
   GLuint shininess;
   GLuint sampler0;
   GLuint sampler1;
   GLuint sampler2;
   GLuint sampler3;
   GLuint sampler4;
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




struct MeshRef: public IRef {
   MeshRef(string name): IRef(name) {
      count = 1;
      loading = true;
   }
   GLuint vertexBuffer;
   GLuint indexBuffer;
   int indexCount;
   int count;
};

struct TextureRef: public IRef {
   TextureRef(string name): IRef(name) {
      count = 1;
      loading = true;
   }
   GLuint textureBuffer;
   int count;
};

// used by the loading thread
enum LOAD_JOB_TYPE {
   LOAD_JOB_MESH,
   LOAD_JOB_TEXTURE,
};

struct LoadingJob {
   LoadingJob(LOAD_JOB_TYPE t, IRef *r) {
      data = NULL;
      type = t;
      ref = r;
   }
   void *data;
   LOAD_JOB_TYPE type;
   IRef *ref;
};

class LoadingJobs {
public:
   LoadingJobs(IResourceManager *resourceManager);
   ~LoadingJobs();
   IResourceManager* GetResourceManager();
   void AddJob(LoadingJob *job);
   LoadingJob* GetJob();
   LoadingJob* GetJobIn();
   void AddJobOut(LoadingJob *job);
private:
   SDL_mutex *lockIn;
   SDL_mutex *lockOut;
   SDL_cond *newJob;
   list<LoadingJob*> in;
   list<LoadingJob*> out;
   SDL_Thread *m_loaderThread;
   IResourceManager *m_resourceManager;
};

int loader_thread(void *jobs);
void CreateSurface(Surface *surface, bool depth);
void BindSurface(Surface *surface);
void ClearSurface();
void CheckFBO();
void CheckError(string call);

#endif
