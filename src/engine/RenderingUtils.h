#ifndef StarRepub_RenderingUtils_h
#define StarRepub_RenderingUtils_h

#define GL_GLEXT_PROTOTYPES

#include "Interfaces.h"
#include "SDL_include.h"
#include "SDL_thread.h"
#include <sstream>

#define VERTEX_STRIDE 11
#define NORMAL_OFFSET 3
#define TEXTURE_OFFSET 9


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

enum FRAME_BUFFER_TYPE {
   FRAME_BUFFER_SCREEN,
   FRAME_BUFFER_PASS0,
   FRAME_BUFFER_PASS1,
   FRAME_BUFFER_REDUCE0,
   FRAME_BUFFER_REDUCE1,
   FRAME_BUFFER_REDUCE2,
   FRAME_BUFFER_REDUCE3,
};

struct FrameBuffer {
   ivec2 bufferSize;
   ivec2 viewSize;
   ivec2 viewLoc;
   vec4 clearColor;
   GLuint texture;
   GLuint depth;
   GLuint fbo;
   FRAME_BUFFER_TYPE type;
};

struct UniformHandles {
   GLuint modelview;
   GLuint projection;
   GLuint normalMatrix;
   GLuint textureMatrix;
   GLuint ambientMaterial;
   GLuint specularMaterial;
   GLuint shininess;
   GLuint *samplers;
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
   int numTextures;
   UniformHandles uniforms;
   AttributeHandles attributes;
};

GLuint buildShader(const char* source, GLenum shaderType);
ShaderProgram buildProgram(const char* vShader, const char* fShader, ShaderProgram &program);
void createFrameBuffer(FrameBuffer &fb, bool depth);
void checkFBO();
void checkError(string call);
void addMesh(MeshData *meshData, MeshRef *meshRef);
void addTexture(TextureData *textureData, TextureRef *textureRef);

#endif
