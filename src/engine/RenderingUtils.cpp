#include "RenderingUtils.h"

GLuint buildShader(const char* source, GLenum shaderType) {
   GLuint shaderHandle = glCreateShader(shaderType);
   glShaderSource(shaderHandle, 1, &source, 0);
   glCompileShader(shaderHandle);
   
   GLint compileSuccess;
   glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
   
   if (compileSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
      std::cout << messages;
      exit(1);
   }
   
   return shaderHandle;
}

ShaderProgram buildProgram(const char* vertexShaderSource, const char* fragmentShaderSource, ShaderProgram &program) {
   program.vertexShader = buildShader(vertexShaderSource, GL_VERTEX_SHADER);
   program.fragmentShader = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
   
   program.program = glCreateProgram();
   glAttachShader(program.program, program.vertexShader);
   glAttachShader(program.program, program.fragmentShader);
   glLinkProgram(program.program);
   
   GLint linkSuccess;
   glGetProgramiv(program.program, GL_LINK_STATUS, &linkSuccess);
   if (linkSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetProgramInfoLog(program.program, sizeof(messages), 0, &messages[0]);
      std::cout << messages;
      exit(1);
   }
   
   program.attributes.position = glGetAttribLocation(program.program, "Position");
   program.attributes.normal = glGetAttribLocation(program.program, "Normal");
   program.attributes.diffuseMaterial = glGetAttribLocation(program.program, "DiffuseMaterial");
   program.attributes.textureCoord = glGetAttribLocation(program.program, "TextureCoord");
   program.uniforms.projection = glGetUniformLocation(program.program, "Projection");
   program.uniforms.modelview = glGetUniformLocation(program.program, "Modelview");
   program.uniforms.normalMatrix = glGetUniformLocation(program.program, "NormalMatrix");
   program.uniforms.textureMatrix = glGetUniformLocation(program.program, "TextureMatrix");
   program.uniforms.ambientMaterial = glGetUniformLocation(program.program, "AmbientMaterial");
   program.uniforms.specularMaterial = glGetUniformLocation(program.program, "SpecularMaterial");
   program.uniforms.shininess = glGetUniformLocation(program.program, "Shininess");
   
   program.uniforms.samplers = new GLuint[program.numTextures];
   for (int i = 0; i < program.numTextures; i++) {
      stringstream ss;
      ss << "Sampler" << i;
      string samplerName = ss.str();
      program.uniforms.samplers[i] = glGetUniformLocation(program.program, samplerName.c_str());
   }
   
   return program;
}

void createFrameBuffer(FrameBuffer &fb, bool depth) {
   // Create the color render buffer
   glGenTextures(1, &fb.texture);
   glBindTexture(GL_TEXTURE_2D, fb.texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fb.bufferSize.x , fb.bufferSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);
   checkError("Creation of the color texture for the FBO");
   
   // Create the depth render buffer
   if (depth) {
      glGenRenderbuffersEXT(1, &fb.depth);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fb.depth);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24,  fb.bufferSize.x,  fb.bufferSize.y);
      checkError("Creation of the depth renderbuffer for the FBO");
   }
   else {
      fb.depth = 0;
   }
   
   // Create the FBO
   glGenFramebuffersEXT(1, &fb.fbo);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb.fbo);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fb.texture, 0);
   if (depth) {
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fb.depth);
   }
   checkFBO();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   checkError("Creation of the FBO");
}

void checkFBO() {
   string enums[] = {
      "attachment",
      "missing attachment",
      "",
      "dimensions",
      "formats",
      "draw buffer",
      "read buffer",
      "unsupported format"
   };
   
   GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
      return;
   status -= GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT;
   cerr << "incomplete framebuffer object due to " << enums[status] << "\n";
   exit(1);
}

void checkError(string call) {
   string enums[] = {
      "invalid enumeration", // GL_INVALID_ENUM
      "invalid value",       // GL_INVALID_VALUE
      "invalid operation",   // GL_INVALID_OPERATION
      "stack overflow",      // GL_STACK_OVERFLOW
      "stack underflow",     // GL_STACK_UNDERFLOW
      "out of memory"        // GL_OUT_OF_MEMORY
   };
   
   GLenum errcode = glGetError();
   if (errcode == GL_NO_ERROR)
      return;
   errcode -= GL_INVALID_ENUM;
   cerr << "OpenGL " << enums[errcode] << " in " << call << "\n";
   exit(1);
}

void addMesh(MeshData *meshData, MeshRef *meshRef) {
   // Add the mesh VBO
   GLuint vertexBuffer;
   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, meshData->vertexCount * VERTEX_STRIDE * sizeof(GLfloat),
                meshData->vertices, GL_STATIC_DRAW);
   
   GLuint indexBuffer;
   glGenBuffers(1, &indexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indexCount * sizeof(GLushort),
                meshData->indices, GL_STATIC_DRAW);
   
   // Update the mesh reference
   meshRef->vertexBuffer = vertexBuffer;
   meshRef->indexBuffer = indexBuffer;
   meshRef->indexCount = meshData->indexCount;
   meshRef->loaded = true;
   meshRef->loading = false;
   
   delete[] meshData->vertices;
   delete[] meshData->indices;
   delete meshData;
}

void addTexture(TextureData *textureData, TextureRef *textureRef) {
   SDL_Surface *surface = (SDL_Surface *) textureData->data;

   int byteSize = surface->format->BytesPerPixel;
   
   // add a new texture. (update for RGBA)
   GLuint textureBuffer;
   glGenTextures(1, &textureBuffer);
   glBindTexture(GL_TEXTURE_2D, textureBuffer);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   if (byteSize == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0,
                   GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
   }
   else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                  GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
   }
   
   
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   
   // Update the texture reference
   textureRef->textureBuffer = textureBuffer;
   textureRef->loaded = true;
   textureRef->loading = false;
   
   delete surface;
   delete textureData;
}