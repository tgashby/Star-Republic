#include "RenderingEngine.h"

float planeVertices[] = {
   0, 0, -1,   0, 0, 1,   0, 0, 0,   0, 0,
   0, 1, -1,   0, 0, 1,   0, 0, 0,   0, 1,
   1, 1, -1,   0, 0, 1,   0, 0, 0,   1, 1,
   1, 0, -1,   0, 0, 1,   0, 0, 0,   1, 0
};

unsigned short planeIndices[] = {
   0, 1, 2,
   0, 2, 3
};

RenderingEngine::RenderingEngine(ivec2 screenSize, Modules *modules) {
   m_screenSize = screenSize;
   m_modules = modules;
   m_camera = NULL;
   TTF_Init();
   m_jobs = new LoadingJobs(m_modules->resourceManager);
   
#ifdef _WIN32
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }
#endif
   
   createShaders();
   useProgram(SHADER_VERTEX_LIGHT);
	
   glEnable(GL_DEPTH_TEST);

   glGenBuffers(1, &m_planeVert);
   glBindBuffer(GL_ARRAY_BUFFER, m_planeVert);
   glBufferData(GL_ARRAY_BUFFER, 4 * VERTEX_STRIDE * sizeof(GLfloat), planeVertices, GL_STATIC_DRAW);
   
   glGenBuffers(1, &m_planeInt);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeInt);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), planeIndices, GL_STATIC_DRAW);

   string fontPath = "fonts/SLEEP.TTF";
   font = TTF_OpenFont(fontPath.c_str(), 40);
   if (!font) {
      cerr << "error reading font\n";
   }

   glBlendFunc(GL_ONE, GL_ONE);
}


RenderingEngine::~RenderingEngine() {
   
}

void RenderingEngine::setCamera(ICamera *camera) {
   m_camera = camera;
}

void RenderingEngine::addObject3d(IObject3d *obj) {
   // get meshes from the new object
   list<IMesh *> *objMeshes = obj->getMeshes();
   list<IMesh *>::iterator mesh;
   for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
      loadMesh(*mesh);
   }
}


void RenderingEngine::removeObject3d(IObject3d *obj) {
   // get meshes from the new object
   list<IMesh *> *objMeshes = obj->getMeshes();
   list<IMesh *>::iterator mesh;
   for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
      unLoadMesh(*mesh);
   }
}

void RenderingEngine::clearScreen() {
   glClearColor(0,0,0,0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void RenderingEngine::drawText(string text, ivec2 loc, ivec2 size) {
   SDL_Surface *initial;
	SDL_Surface *intermediary;
	int w,h;
	
   // get a font
   SDL_Color color = {255, 255, 255};
   
   
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text.c_str(), color);
	assert(initial != NULL);
	/* Convert the rendered text to a known format */
	w = 512;
	h = 35;
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
                                       0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
        assert(intermediary != NULL);
	SDL_BlitSurface(initial, NULL, intermediary, NULL);
        assert(intermediary != NULL);
   
   // Load a new texture.
   GLuint textureBuffer;
   glGenTextures(1, &textureBuffer);
   glBindTexture(GL_TEXTURE_2D, textureBuffer);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0,
                GL_BGRA, GL_UNSIGNED_BYTE, intermediary->pixels);
   
   
   // Draw the Text
   glDisable(GL_DEPTH);
   glEnable(GL_BLEND);
   
   mat4 projection = mat4::Parallel(-400, 400, -300, 300, 1, 10);
   glUniformMatrix4fv(m_curShaderProgram->uniforms.projection, 1, 0, projection.Pointer());
   
   // Set the model view matrix
   mat4 modelMtx = mat4::Scale(size.x, -size.y, 1);
   modelMtx = modelMtx * mat4::Translate(loc.x, loc.y, 0);
   glUniformMatrix4fv(m_curShaderProgram->uniforms.modelview, 1, 0, modelMtx.Pointer());
   
   // Set the normal matrix
   mat3 normalMtx = modelMtx.ToMat3();
   glUniformMatrix3fv(m_curShaderProgram->uniforms.normalMatrix, 1, 0, normalMtx.Pointer());
   
   
	int stride = 11 * sizeof(GLfloat);
   const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
   const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
   GLint position = m_curShaderProgram->attributes.position;
   GLint normal = m_curShaderProgram->attributes.normal;
   GLint texCoord = m_curShaderProgram->attributes.textureCoord;
   
   glBindTexture(GL_TEXTURE_2D, textureBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, m_planeVert);
   glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, 0);
   glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, stride, normalOffset);
   glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, stride, texCoordOffset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeInt);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	
	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &textureBuffer);
   
   
   glEnable(GL_DEPTH);
   glDisable(GL_BLEND);
}

void RenderingEngine::render(list<IObject3d *> &objects) {
   processJobs();
   
   glClearColor(0,0,0,0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   
   // Get the projection * view matrix from the camera.
   mat4 projectionViewMatrix = m_camera->getProjectionViewMtx();
   
   // Set the light position.
   vec4 lightPosition(1, 1, 1, 0);
   
   list<IObject3d *>::iterator obj;
   list<IMesh *> *objMeshes;
   list<IMesh *>::iterator mesh;
   
   for (obj = objects.begin(); obj != objects.end(); ++obj) {
      objMeshes = (*obj)->getMeshes();
      for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
         // Skip the mesh if it is not visible or is not loaded yet.
         if (!(*mesh)->isVisible() || !(*mesh)->checkLoaded())
            continue;
         
         // Check if the mesh was loaded.
         IRef *ref;
         ref = (*mesh)->getMeshRef();
         if (!ref->loaded) {
            continue;
         }
         MeshRef *meshRef = (MeshRef *) ref;
         
         // Check if the texture was loaded.
         ref = (*mesh)->getTextureRef(0);
         if (!ref->loaded) {
            continue;
         }
         TextureRef *textureRef = (TextureRef *) ref;
         
         // Change the shading program when needed
         if ((*mesh)->getShaderType() != m_curShaderProgram->type) {
            useProgram((*mesh)->getShaderType());
         }
         glUniformMatrix4fv(m_curShaderProgram->uniforms.projection, 1, 0, projectionViewMatrix.Pointer());
         glUniform3fv(m_curShaderProgram->uniforms.lightPosition, 1, lightPosition.Pointer());
         
         // Set the model view matrix
         mat4 modelMtx = mat4::Scale((*mesh)->getScale());
         modelMtx = modelMtx * (*mesh)->getModelMtx() ;
         glUniformMatrix4fv(m_curShaderProgram->uniforms.modelview, 1, 0, modelMtx.Pointer());
         
         // Set the normal matrix
         mat3 normalMtx = (*mesh)->getModelMtx().ToMat3();
         glUniformMatrix3fv(m_curShaderProgram->uniforms.normalMatrix, 1, 0, normalMtx.Pointer());
         
         // Set the diffuse color.
         vec4 color =(*mesh)->getColor();
         glVertexAttrib4f(m_curShaderProgram->attributes.diffuseMaterial, color.x, color.y, color.z, color.w);
         
         // Draw the surface.
         int stride = 11 * sizeof(GLfloat);
         const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
         const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
         GLint position = m_curShaderProgram->attributes.position;
         GLint normal = m_curShaderProgram->attributes.normal;
         GLint texCoord = m_curShaderProgram->attributes.textureCoord;
         
         glBindTexture(GL_TEXTURE_2D, textureRef->textureBuffer);
         glBindBuffer(GL_ARRAY_BUFFER, meshRef->vertexBuffer);
         glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, 0);
         glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, stride, normalOffset);
         glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, stride, texCoordOffset);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRef->indexBuffer);
         glDrawElements(GL_TRIANGLES, meshRef->indexCount, GL_UNSIGNED_SHORT, 0);
      }
   }
}

void RenderingEngine::processJobs() {
   LoadingJob *curJob = m_jobs->GetJob();
   if (curJob == NULL) {
      return;
   }
   
   //cout << "add: " << curJob->ref->fileName << "\n";
   
   switch (curJob->type) {
      case LOAD_JOB_MESH:
         addMesh((MeshData*) curJob->data, (MeshRef*) curJob->ref);
         break;
         
      case LOAD_JOB_TEXTURE:
         addTexture((TextureData*) curJob->data, (TextureRef*) curJob->ref);
         break;
         
      default:
         cerr << "Unknown load job\n";
         exit(1);
         break;
   }
   delete curJob;
}

void RenderingEngine::addMesh(MeshData *meshData, MeshRef *meshRef) {
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

void RenderingEngine::addTexture(TextureData *textureData, TextureRef *textureRef) {
   SDL_Surface *surface = (SDL_Surface *) textureData->data;
   
   // add a new texture. (update for RGBA)
   GLuint textureBuffer;
   glGenTextures(1, &textureBuffer);
   glBindTexture(GL_TEXTURE_2D, textureBuffer);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0,
                GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   
   // Update the texture reference
   textureRef->textureBuffer = textureBuffer;
   textureRef->loaded = true;
   textureRef->loading = false;
   
   delete surface;
   delete textureData;
}

GLuint RenderingEngine::buildShader(const char* source, GLenum shaderType) const {
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

ShaderProgram RenderingEngine::buildProgram(const char* vertexShaderSource, const char* fragmentShaderSource, SHADER_TYPE type) {
   ShaderProgram program;
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
   program.uniforms.lightPosition = glGetUniformLocation(program.program, "LightPosition");
   program.uniforms.ambientMaterial = glGetUniformLocation(program.program, "AmbientMaterial");
   program.uniforms.specularMaterial = glGetUniformLocation(program.program, "SpecularMaterial");
   program.uniforms.shininess = glGetUniformLocation(program.program, "Shininess"); 
   program.uniforms.sampler = glGetUniformLocation(program.program, "Sampler");
   
   return program;
}

void RenderingEngine::createShaders() {
   ShaderProgram newShader;
   string vertexShader, fragmentShader;
   
   vertexShader = m_modules->resourceManager->readShader("shaders/VertexLighting.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/VertexLighting.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), SHADER_VERTEX_LIGHT);
   m_shaderPrograms[SHADER_VERTEX_LIGHT] = newShader;
   
   vertexShader = m_modules->resourceManager->readShader("shaders/NoLighting.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/NoLighting.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), SHADER_NO_LIGHT);
   m_shaderPrograms[SHADER_NO_LIGHT] = newShader;
}

void RenderingEngine::useProgram(SHADER_TYPE type) {
   m_curShaderProgram = &m_shaderPrograms[type];
   glUseProgram(m_curShaderProgram->program);
   
   // Set up some default material properties 
   glUniform3f(m_curShaderProgram->uniforms.ambientMaterial, 0.4f, 0.4f, 0.4f);
   glUniform3f(m_curShaderProgram->uniforms.specularMaterial, 0.5, 0.5, 0.5);
   glUniform1f(m_curShaderProgram->uniforms.shininess, 50);
   
   // setup vertex attributes for current program.
   glEnableVertexAttribArray(m_curShaderProgram->attributes.position);
   glEnableVertexAttribArray(m_curShaderProgram->attributes.normal);
   glEnableVertexAttribArray(m_curShaderProgram->attributes.textureCoord);
   
   // set a texture
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(m_curShaderProgram->uniforms.sampler, 0);
}

void RenderingEngine::loadMesh(IMesh *newMesh) {
   // Check if the mesh was already loaded
   IRef *ref = newMesh->getMeshRef();
   if (!ref->loaded && !ref->loading) {
      map<string, MeshRef*>::iterator meshIter = m_meshMap.find(ref->fileName);
      if (meshIter != m_meshMap.end()) {
         // Found a MeshRef for newMesh to reference
         newMesh->setMeshRef(meshIter->second);
         meshIter->second->count += 1;
      }
      else {
         cout << "load: " << ref->fileName << "\n";
         MeshRef *newMeshRef = new MeshRef(ref->fileName);
         m_meshMap[ref->fileName] = newMeshRef;
         newMesh->setMeshRef(newMeshRef);
         m_jobs->AddJob(new LoadingJob(LOAD_JOB_MESH, newMeshRef));
         /*
         // Get the meshData.
         MeshData *meshData = m_modules->resourceManager->readMeshData(ref->fileName, LOAD_NORMAL_VERTEX, 1.0);;
         
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
         
         MeshRef *newMeshRef = new MeshRef(ref->fileName, vertexBuffer, indexBuffer, meshData->indexCount, meshData->bounds);
         newMesh->setMeshRef(newMeshRef);
         m_meshMap[ref->fileName] = newMeshRef;
         
         // Clean up the mesh data
         delete[] meshData->vertices;
         delete[] meshData->indices;
         delete meshData;
          */
      }
      delete ref;
   }
   
   vector<IRef*> *refs = newMesh->getTextureRefs();
   vector<IRef*>::iterator refIter = refs->begin();
   while (refIter != refs->end()) {
      ref = *refIter;
      
      if (!ref->loaded && !ref->loading) {
         map<string, TextureRef*>::iterator textureIter = m_textureMap.find(ref->fileName);
         if (textureIter != m_textureMap.end()) {
            *refIter = textureIter->second;
            textureIter->second->count += 1;
         }
         else {
            TextureRef *newTextureRef = new TextureRef(ref->fileName);
            m_textureMap[ref->fileName] = newTextureRef;
            *refIter = newTextureRef;
            m_jobs->AddJob(new LoadingJob(LOAD_JOB_TEXTURE, newTextureRef));
            /*
            // Get the texture
            TextureData *textureData = m_modules->resourceManager->loadBMPImage(ref->fileName);
            SDL_Surface *surface = (SDL_Surface *) textureData->data;
            
            // Load a new texture.
            GLuint textureBuffer;
            glGenTextures(1, &textureBuffer);
            glBindTexture(GL_TEXTURE_2D, textureBuffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0,
                         GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->size.x, textureData->size.y, 0,
            //               GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);
            delete textureData;
            glGenerateMipmapEXT(GL_TEXTURE_2D);
            
            // Setup a new texture reference
            TextureRef *newTextureRef = new TextureRef(ref->fileName, textureBuffer);
            *refIter = newTextureRef;
            m_textureMap[ref->fileName] = newTextureRef;*/
         }
         delete ref;
      }
      
      ++refIter;
   }
}

void RenderingEngine::unLoadMesh(IMesh* rmvMesh) {
   IRef *ref = rmvMesh->getMeshRef();
   rmvMesh->setMeshRef(new IRef(ref->fileName));
   
   map<string, MeshRef*>::iterator meshIter = m_meshMap.find(ref->fileName);
   if (meshIter != m_meshMap.end()) {
      meshIter->second->count -= 1;
      if (meshIter->second->count < 1) {
         glDeleteBuffers(1, &meshIter->second->vertexBuffer);
         glDeleteBuffers(1, &meshIter->second->indexBuffer);
         delete meshIter->second;
         m_meshMap.erase(meshIter);
      }
   }
   else {
      cout << "Mesh " << ref->fileName << " was already unloaded from the RenderingEngine\n";
      delete ref;
   }
   
   
   vector<IRef*> *refs = rmvMesh->getTextureRefs();
   vector<IRef*>::iterator refIter = refs->begin();
   while (refIter != refs->end()) {
      ref = *refIter;
      *refIter = new IRef(ref->fileName);
      
      map<string, TextureRef*>::iterator textureIter = m_textureMap.find(ref->fileName);
      if (textureIter != m_textureMap.end()) {
         textureIter->second->count -= 1;
         if (textureIter->second->count < 1) {
            glDeleteTextures(1, &textureIter->second->textureBuffer);
            delete textureIter->second;
            m_textureMap.erase(textureIter);
         }
      }
      else {
         cout << "textrue " << ref->fileName << " was already unloaded from the RenderingEngine\n";
         delete ref;
      }
      
      ++refIter;
   }
}



LoadingJobs::LoadingJobs(IResourceManager *resourceManager) {
   m_resourceManager = resourceManager;
   lockIn = SDL_CreateMutex();
   lockOut = SDL_CreateMutex();
   newJob = SDL_CreateCond();
   in = list<LoadingJob*>(0);
   out = list<LoadingJob*>(0);
   m_loaderThread = SDL_CreateThread(loader_thread, this);
   if (m_loaderThread == NULL) {
      cerr << "Problem starting loader thread";
      exit(1);
   }
}

IResourceManager* LoadingJobs::GetResourceManager() {
   return m_resourceManager;
}

LoadingJobs::~LoadingJobs() {
   SDL_KillThread(m_loaderThread);
   SDL_DestroyMutex(lockIn);
   SDL_DestroyMutex(lockOut);
   SDL_DestroyCond(newJob);
}

void LoadingJobs::AddJob(LoadingJob *job) {
   SDL_mutexP(lockIn);
   in.push_back(job);
   SDL_mutexV(lockIn);
   SDL_CondSignal(newJob);
}

LoadingJob* LoadingJobs::GetJob() {
   SDL_mutexP(lockOut);
   LoadingJob *job = NULL;
   if (out.size() != 0) {
      job = out.front();
      out.pop_front();
   }
   SDL_mutexV(lockOut);
   return job;
}

LoadingJob* LoadingJobs::GetJobIn() {
   SDL_mutexP(lockIn);
   if (in.size() == 0) {
      cout << "stop loading thread\n";
      SDL_CondWait(newJob, lockIn);
      cout << "start loading thread\n";
   }
   LoadingJob *job = in.front();
   in.pop_front();
   SDL_mutexV(lockIn);
   return job;
}

void LoadingJobs::AddJobOut(LoadingJob *job) {
   SDL_mutexP(lockOut);
   out.push_back(job);
   SDL_mutexV(lockOut);
}

int loader_thread(void *jobs) {
   LoadingJobs *loadingJobs = (LoadingJobs *) jobs;
   IResourceManager *resourceManager = loadingJobs->GetResourceManager();
   LoadingJob *curJob;
   while (true) {
      // get a new job from the in list.
      curJob = loadingJobs->GetJobIn();
      
      // process the job
      switch (curJob->type) {
         case LOAD_JOB_MESH:
            // load a mesh
            curJob->data = resourceManager->readMeshData(curJob->ref->fileName, LOAD_NORMAL_VERTEX, 1.0);
            break;
            
         case LOAD_JOB_TEXTURE:
            // load a texture
            curJob->data = resourceManager->loadBMPImage(curJob->ref->fileName);
            break;
            
         default:
            cerr << "Unknown load job\n";
            exit(1);
            break;
      }
      
      // put the finished job in the out list.
      loadingJobs->AddJobOut(curJob);
   }
   return 0;
}