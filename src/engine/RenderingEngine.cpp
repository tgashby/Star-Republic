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
   m_curShaderProgram = NULL;
   useProgram(SHADER_VERTEX_LIGHT);
	
   glEnable(GL_DEPTH_TEST);

   // Create a plane
   glGenBuffers(1, &m_planeVert);
   glBindBuffer(GL_ARRAY_BUFFER, m_planeVert);
   glBufferData(GL_ARRAY_BUFFER, 4 * VERTEX_STRIDE * sizeof(GLfloat), planeVertices, GL_STATIC_DRAW);
   
   glGenBuffers(1, &m_planeInt);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeInt);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), planeIndices, GL_STATIC_DRAW);
   
   // Setup the plane
   m_planeMesh = new Mesh("plane", "planeTexture", m_modules);
   m_planeMesh->setScale3v(vec3(800, 600, 1));
   m_planeMesh->setModelMtx(mat4::Translate(-400, -300, 0));
   MeshRef *meshRef = new MeshRef("plane");
   meshRef->vertexBuffer = m_planeVert;
   meshRef->indexBuffer = m_planeInt;
   meshRef->indexCount = 6;
   m_planeMesh->setMeshRef(meshRef);
   m_planeTextures = m_planeMesh->getTextureRefs();
   (*m_planeTextures)[0] = (IRef*) new TextureRef("texture0");
   m_planeTextures->push_back(new TextureRef("texture1"));
   m_planeTextures->push_back(new TextureRef("texture2"));
   m_planeTextures->push_back(new TextureRef("texture3"));
   m_planeTextures->push_back(new TextureRef("texture4"));
   
   

   string fontPath = "fonts/SLEEP.TTF";
   font = TTF_OpenFont(fontPath.c_str(), 40);
   if (!font) {
      cerr << "error reading font\n";
   }

   glBlendFunc(GL_ONE, GL_ONE);
   
   ivec2 size = m_screenSize;
   // create the frambuffers
   pass0.size = size;
   pass0.clearColor = vec4(0.0, 0.0, 0.0, 1.0);
   pass0.viewport.loc = ivec2(0, 0);
   pass0.viewport.size = size;
   CreateSurface(&pass0, true);
   pass1.size = size;
   pass1.clearColor = vec4(0.0, 0.0, 0.0, 1.0);
   pass1.viewport.loc = ivec2(0, 0);
   pass1.viewport.size = size;
   CreateSurface(&pass1, true);
   
   size = size / 2;
   reduce0.size = size;
   reduce0.clearColor = vec4(0.0, 0.0, 0.0, 1.0);
   reduce0.viewport.loc = ivec2(0, 0);
   reduce0.viewport.size = size;
   CreateSurface(&reduce0, true);
   
   size = size / 2;
   reduce1.size = size;
   reduce1.clearColor = vec4(0.0, 0.0, 0.0, 1.0);
   reduce1.viewport.loc = ivec2(0, 0);
   reduce1.viewport.size = size;
   CreateSurface(&reduce1, true);
   
   size = size / 2;
   reduce2.size = size;
   reduce2.clearColor = vec4(0.0, 0.0, 0.0, 1.0);
   reduce2.viewport.loc = ivec2(0, 0);
   reduce2.viewport.size = size;
   CreateSurface(&reduce2, true);
   
   size = size / 2;
   reduce3.size = size;
   reduce3.clearColor = vec4(0.0, 0.0, 0.0, 1.0);
   reduce3.viewport.loc = ivec2(0, 0);
   reduce3.viewport.size = size;
   CreateSurface(&reduce3, true);
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
   
   BindSurface(&pass0);
   
   glClearColor(0,0,0,0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   
   // Get the projection * view matrix from the camera.
   mat4 projectionViewMatrix = m_camera->getProjectionViewMtx();
   
   // Set the light position.
   vec4 lightPosition(1, 1, 1, 0);
   
   list<IObject3d *>::iterator obj;
   list<IMesh *> *objMeshes;
   list<IMesh *>::iterator mesh;
   
   vec3 scale;
   
   for (obj = objects.begin(); obj != objects.end(); ++obj) {
      objMeshes = (*obj)->getMeshes();
      for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
         // Skip the mesh if it is not visible or is not loaded yet.
         if (!(*mesh)->isVisible() || !(*mesh)->checkLoaded())
            continue;
         
         // Draw the mesh
         drawMesh((*mesh), projectionViewMatrix);
      }
   }
   
   glDisable(GL_DEPTH);
   glEnable(GL_BLEND);
   
   projectionViewMatrix = mat4::Parallel(-400, 400, -300, 300, 1, 10);
   TextureRef *texture0 = (TextureRef*)(*m_planeTextures)[0];
   TextureRef *texture1 = (TextureRef*)(*m_planeTextures)[1];
   TextureRef *texture2 = (TextureRef*)(*m_planeTextures)[2];
   TextureRef *texture3 = (TextureRef*)(*m_planeTextures)[3];
   TextureRef *texture4 = (TextureRef*)(*m_planeTextures)[4];
   
   BindSurface(&pass1);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   m_planeMesh->setShaderType(SHADER_BLOOM_CULL);
   texture0->textureBuffer = pass0.texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix);
   
   BindSurface(&reduce0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = pass1.texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix);
   
   BindSurface(&reduce1);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = reduce0.texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix);
   
   BindSurface(&reduce2);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = reduce1.texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix);
   
   BindSurface(&reduce3);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = reduce2.texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix);
   
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   glViewport(0, 0, m_screenSize.x, m_screenSize.y);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   m_planeMesh->setShaderType(SHADER_COMBINE);
   //m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   //texture0->textureBuffer = reduce3.texture;
   texture0->textureBuffer = pass0.texture;
   texture1->textureBuffer = reduce0.texture;
   texture2->textureBuffer = reduce1.texture;
   texture3->textureBuffer = reduce2.texture;
   texture4->textureBuffer = reduce3.texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix);
   
   glEnable(GL_DEPTH);
   glDisable(GL_BLEND);
}

void RenderingEngine::drawMesh(IMesh *mesh, mat4 projection) {
   
   // Change the shading program when needed
   if (mesh->getShaderType() != m_curShaderProgram->type) {
      useProgram(mesh->getShaderType());
   }
   
   glUniformMatrix4fv(m_curShaderProgram->uniforms.projection, 1, 0, projection.Pointer());
   glUniform3fv(m_curShaderProgram->uniforms.lightPosition, 1, vec4(1, 1, 1, 0).Pointer());
   
   MeshRef *meshRef = (MeshRef*) mesh->getMeshRef();
   TextureRef *textureRef = (TextureRef*) mesh->getTextureRef(0);
   
   // Set the model view matrix
   vec3 scale = mesh->getScale();
   mat4 modelMtx = mat4::Scale(scale.x, scale.y, scale.z);
   modelMtx = modelMtx * mesh->getModelMtx();
   glUniformMatrix4fv(m_curShaderProgram->uniforms.modelview, 1, 0, modelMtx.Pointer());
   
   // Set the normal matrix
   mat3 normalMtx = mesh->getModelMtx().ToMat3();
   glUniformMatrix3fv(m_curShaderProgram->uniforms.normalMatrix, 1, 0, normalMtx.Pointer());
   
   // Set the diffuse color.
   vec4 color = vec4(0.8, 0.8, 0.8, 1.0);
   glVertexAttrib4f(m_curShaderProgram->attributes.diffuseMaterial, color.x, color.y, color.z, color.w);
   
   // Draw the surface.
   int stride = 11 * sizeof(GLfloat);
   const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
   const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
   GLint position = m_curShaderProgram->attributes.position;
   GLint normal = m_curShaderProgram->attributes.normal;
   GLint texCoord = m_curShaderProgram->attributes.textureCoord;
   
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textureRef->textureBuffer);
   glEnable(GL_TEXTURE_2D);
   
   if (mesh->getShaderType() == SHADER_COMBINE) {
      glActiveTexture(GL_TEXTURE1);
      textureRef = (TextureRef*) mesh->getTextureRef(1);
      glBindTexture(GL_TEXTURE_2D, textureRef->textureBuffer);
      glEnable(GL_TEXTURE_2D);
      
      glActiveTexture(GL_TEXTURE2);
      textureRef = (TextureRef*) mesh->getTextureRef(2);
      glBindTexture(GL_TEXTURE_2D, textureRef->textureBuffer);
      glEnable(GL_TEXTURE_2D);
      
      glActiveTexture(GL_TEXTURE3);
      textureRef = (TextureRef*) mesh->getTextureRef(3);
      glBindTexture(GL_TEXTURE_2D, textureRef->textureBuffer);
      glEnable(GL_TEXTURE_2D);
      
      glActiveTexture(GL_TEXTURE4);
      textureRef = (TextureRef*) mesh->getTextureRef(4);
      glBindTexture(GL_TEXTURE_2D, textureRef->textureBuffer);
      glEnable(GL_TEXTURE_2D);
   }
   
   
   glBindBuffer(GL_ARRAY_BUFFER, meshRef->vertexBuffer);
   glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, 0);
   glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, stride, normalOffset);
   glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, stride, texCoordOffset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRef->indexBuffer);
   glDrawElements(GL_TRIANGLES, meshRef->indexCount, GL_UNSIGNED_SHORT, 0);
}

void RenderingEngine::processJobs() {
   int startTime = SDL_GetTicks();
   LoadingJob *curJob;
   
   do {
      // Get a job.
      curJob = m_jobs->GetJob();
      if (curJob == NULL) {
         return; // If no job leave.
      }
      
      // Process the job and cleanup.
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
      
   } while (PROCESS_TIME > SDL_GetTicks() - startTime);
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
   program.type = type;
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
   program.uniforms.sampler0 = glGetUniformLocation(program.program, "Sampler");
   if (type == SHADER_COMBINE) {
      program.uniforms.sampler1 = glGetUniformLocation(program.program, "Sampler1");
      program.uniforms.sampler2 = glGetUniformLocation(program.program, "Sampler2");
      program.uniforms.sampler3 = glGetUniformLocation(program.program, "Sampler3");
      program.uniforms.sampler4 = glGetUniformLocation(program.program, "Sampler4");
   }
   
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
   
   vertexShader = m_modules->resourceManager->readShader("shaders/Bloom.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/Bloom.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), SHADER_BLOOM);
   m_shaderPrograms[SHADER_BLOOM] = newShader;
   
   vertexShader = m_modules->resourceManager->readShader("shaders/BloomCull.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/BloomCull.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), SHADER_BLOOM_CULL);
   m_shaderPrograms[SHADER_BLOOM_CULL] = newShader;
   
   vertexShader = m_modules->resourceManager->readShader("shaders/Combine.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/Combine.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), SHADER_COMBINE);
   m_shaderPrograms[SHADER_COMBINE] = newShader;
}

void RenderingEngine::useProgram(SHADER_TYPE type) {
   if (m_curShaderProgram != NULL && type == m_curShaderProgram->type) {
      return;
   }
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
   glUniform1i(m_curShaderProgram->uniforms.sampler0, 0);
   
   // set up extra textures for combine
   if (type == SHADER_COMBINE) {
      glActiveTexture(GL_TEXTURE1);
      glUniform1i(m_curShaderProgram->uniforms.sampler1, 1);
      glActiveTexture(GL_TEXTURE2);
      glUniform1i(m_curShaderProgram->uniforms.sampler2, 2);
      glActiveTexture(GL_TEXTURE3);
      glUniform1i(m_curShaderProgram->uniforms.sampler3, 3);
      glActiveTexture(GL_TEXTURE4);
      glUniform1i(m_curShaderProgram->uniforms.sampler4, 4);
   }
}

void RenderingEngine::loadMesh(IMesh *newMesh) {
   // Check if the mesh was already loaded
   IRef *ref = newMesh->getMeshRef();
   cout << "load: " << ref->fileName << "\n";
   if (!ref->loaded && !ref->loading) {
      map<string, MeshRef*>::iterator meshIter = m_meshMap.find(ref->fileName);
      if (meshIter != m_meshMap.end()) {
         // Found a MeshRef for newMesh to reference
         newMesh->setMeshRef(meshIter->second);
         meshIter->second->count += 1;
      }
      else {
         //cout << "load: " << ref->fileName << "\n";
         MeshRef *newMeshRef = new MeshRef(ref->fileName);
         m_meshMap[ref->fileName] = newMeshRef;
         newMesh->setMeshRef(newMeshRef);
         m_jobs->AddJob(new LoadingJob(LOAD_JOB_MESH, newMeshRef));
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
