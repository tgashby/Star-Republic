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
   m_loader = new ResourceLoader(m_modules->resourceManager);
   createShaders();
   createFrameBuffers();
   
   // setup font
   TTF_Init();
   string fontPath = "fonts/SLEEP.TTF";
   m_font = TTF_OpenFont(fontPath.c_str(), 40);
   if (!m_font) {
      cerr << "error reading font\n";
      exit(1);
   }
   
   // Setup the plane (remove when deligated to its own class)
   glGenBuffers(1, &m_planeVert);
   glBindBuffer(GL_ARRAY_BUFFER, m_planeVert);
   glBufferData(GL_ARRAY_BUFFER, 4 * VERTEX_STRIDE * sizeof(GLfloat), planeVertices, GL_STATIC_DRAW);
   glGenBuffers(1, &m_planeInt);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeInt);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), planeIndices, GL_STATIC_DRAW);
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
   
   //Set a Light
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   vec4 lightPos(1, 1, 1, 1);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos.Pointer());
   
   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_ONE, GL_ONE);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);
}

RenderingEngine::~RenderingEngine() {
   
}

void RenderingEngine::setCamera(ICamera *camera) {
   m_camera = camera;
}

void RenderingEngine::addObject3d(IObject3d *obj) {
   list<IMesh *> *objMeshes = obj->getMeshes();
   list<IMesh *>::iterator mesh;
   for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
      loadMesh(*mesh);
   }
}

void RenderingEngine::removeObject3d(IObject3d *obj) {
   list<IMesh *> *objMeshes = obj->getMeshes();
   list<IMesh *>::iterator mesh;
   for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
      unLoadMesh(*mesh);
   }
}

void RenderingEngine::render(list<IObject3d *> &objects) {

   list<IMesh *> bloomedMeshes(0);
   
   addLoaded();
   
   setFrameBuffer(FRAME_BUFFER_PASS1);
   clearScreen();
   
   // Get the projection * view matrix from the camera.
   mat4 projectionViewMatrix1 = m_camera->getProjectionViewMtx();
   mat4 projectionViewMatrix2 = mat4::Parallel(-400, 400, -300, 300, 1, 10);
   TextureRef *texture0 = (TextureRef*)(*m_planeTextures)[0];
   TextureRef *texture1 = (TextureRef*)(*m_planeTextures)[1];
   TextureRef *texture2 = (TextureRef*)(*m_planeTextures)[2];
   TextureRef *texture3 = (TextureRef*)(*m_planeTextures)[3];
   TextureRef *texture4 = (TextureRef*)(*m_planeTextures)[4];
   
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
         
         // Check if the mesh is a bloom mesh. put into list for the second pass.
         if ((*mesh)->getShaderType() == SHADER_BLOOM) {
            bloomedMeshes.push_back(*mesh);
            continue;
         }
         
         // Draw the mesh
         drawMesh((*mesh), projectionViewMatrix1);
      }
   }
   
   glDisable(GL_DEPTH);
   glEnable(GL_BLEND);
   
   setFrameBuffer(FRAME_BUFFER_PASS0);
   clearScreen();
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = m_frameBuffers[FRAME_BUFFER_PASS1].texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix2);
   
   glEnable(GL_DEPTH);
   glDisable(GL_BLEND);
   
   // Clear only the color buffer. Keep the depth for occluding bloomed meshes.
   setFrameBuffer(FRAME_BUFFER_PASS1);
   glClearColor(0,0,0,1);
   glClear(GL_COLOR_BUFFER_BIT); // clear only the color buffer.
   
   // Draw the bloomed meshes.
   for (mesh = bloomedMeshes.begin(); mesh != bloomedMeshes.end(); ++mesh) {      
      // Draw the mesh
      drawMesh((*mesh), projectionViewMatrix1);
   }
   
   glDisable(GL_DEPTH);
   glEnable(GL_BLEND);
   
   setFrameBuffer(FRAME_BUFFER_REDUCE0);
   clearScreen();
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = m_frameBuffers[FRAME_BUFFER_PASS1].texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix2);
   
   setFrameBuffer(FRAME_BUFFER_REDUCE1);
   clearScreen();
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE0].texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix2);

   setFrameBuffer(FRAME_BUFFER_REDUCE2);
   clearScreen();
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE1].texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix2);
   
   setFrameBuffer(FRAME_BUFFER_REDUCE3);
   clearScreen();
   m_planeMesh->setShaderType(SHADER_NO_LIGHT);
   texture0->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE2].texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix2);
   
   setFrameBuffer(FRAME_BUFFER_SCREEN);
   clearScreen();
   m_planeMesh->setShaderType(SHADER_COMBINE);
   texture0->textureBuffer = m_frameBuffers[FRAME_BUFFER_PASS0].texture;
   texture1->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE0].texture;
   texture2->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE1].texture;
   texture3->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE2].texture;
   texture4->textureBuffer = m_frameBuffers[FRAME_BUFFER_REDUCE3].texture;
   drawMesh((IMesh*) m_planeMesh, projectionViewMatrix2);
   
   glEnable(GL_DEPTH);
   glDisable(GL_BLEND);
   
   //MY CODE BELOW HERE
   //glBegin(GL_POLYGON);
   //glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   //glOrtho(0, 800, 0, 500, -1, 1);

   glEnable(GL_BLEND);
   glDisable(GL_DEPTH);
   
   
   glDisable(GL_DEPTH_TEST);

   //glClear(GL_COLOR_BUFFER_BIT);
   //glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
   //glEnable(GL_COLOR_MATERIAL);
   
   //glDisable(GL_CULL_FACE);
   //glDisable(GL_TEXTURE_2D);
   //glDisable(GL_LIGHTING);
   //glClear(GL_COLOR_BUFFER_BIT);//| GL_DEPTH_BUFFER_BIT);
   //glFlush();
   //glEnable(GL_LIGHTING);
   glColor4f(0.5, 0.5, 0.5, 1.0);
   glRectf(.05, .04, .6, .09);
   glColor4f(.24, 1.0, .69, 1.0);
   //glRectf(.08, .05, .57, .08);
   glRectf(.08, .05, .08 + (.49 * getPercentHealth()), .08);

   //glDisable(GL_COLOR_MATERIAL);
   
   //SDL_GL_SwapBuffers();
   glPopMatrix();
   //glEnable(GL_DEPTH_TEST);

   glDisable(GL_BLEND);
   glEnable(GL_DEPTH);
   glEnable(GL_DEPTH_TEST);
   //glEnd();
   //SDL_GL_SwapBuffers();
   
   //glEnd();
   //glutSwapBuffers();
   //END MY CODE
}

float RenderingEngine::getPercentHealth() {
   return .5;
}

void RenderingEngine::drawText(string text, ivec2 loc, ivec2 size) {
   SDL_Surface *initial;
	SDL_Surface *intermediary;
	int w,h;
	
   // get a font
   SDL_Color color = {255, 255, 255};
   
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(m_font, text.c_str(), color);
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
   
   setShaderProgram(SHADER_NO_LIGHT);
   
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
   
   // Set the texture matrix
   mat4 textureMtx = mat4::Identity();
   glUniformMatrix4fv(m_curShaderProgram->uniforms.textureMatrix, 1, 0, textureMtx.Pointer());
   
   
	int stride = 11 * sizeof(GLfloat);
   const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
   const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
   GLint position = m_curShaderProgram->attributes.position;
   GLint normal = m_curShaderProgram->attributes.normal;
   GLint texCoord = m_curShaderProgram->attributes.textureCoord;
   
   glBindTexture(GL_TEXTURE_2D, textureBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, m_planeVert );
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

void RenderingEngine::clearScreen() {
   glClearColor(0,0,0,1);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}


void RenderingEngine::drawMesh(IMesh *mesh, mat4 projection) {
   
   // Change the shading program when needed
   if (mesh->getShaderType() != m_curShaderProgram->type) {
      setShaderProgram(mesh->getShaderType());
   }
   
   glUniformMatrix4fv(m_curShaderProgram->uniforms.projection, 1, 0, projection.Pointer());
   
   MeshRef *meshRef = (MeshRef*) mesh->getMeshRef();
   TextureRef *textureRef;
   
   // Set the model view matrix
   vec3 scale = mesh->getScale();
   mat4 modelMtx = mat4::Scale(scale.x, scale.y, scale.z);
   modelMtx = modelMtx * mesh->getModelMtx();
   glUniformMatrix4fv(m_curShaderProgram->uniforms.modelview, 1, 0, modelMtx.Pointer());
   
   // Set the normal matrix
   mat3 normalMtx = mesh->getModelMtx().ToMat3();
   glUniformMatrix3fv(m_curShaderProgram->uniforms.normalMatrix, 1, 0, normalMtx.Pointer());
   
   // Set the texture matrix
   mat4 textureMtx = mesh->getTextureMtx();
   glUniformMatrix4fv(m_curShaderProgram->uniforms.textureMatrix, 1, 0, textureMtx.Pointer());
   
   // Draw the surface.
   int stride = 11 * sizeof(GLfloat);
   const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
   const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
   GLint position = m_curShaderProgram->attributes.position;
   GLint normal = m_curShaderProgram->attributes.normal;
   GLint texCoord = m_curShaderProgram->attributes.textureCoord;
   
   for (int i = 0; i < m_curShaderProgram->numTextures; i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      textureRef = (TextureRef*) mesh->getTextureRef(i);
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

void RenderingEngine::addLoaded() {
   int startTime = SDL_GetTicks();
   LoadingJob *curJob;
   do {
      // Get a job.
      curJob = m_loader->getJobOut();
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

void RenderingEngine::loadMesh(IMesh *newMesh) {
   // Check if the mesh was already loaded
   IRef *ref = newMesh->getMeshRef();
   //cout << "load: " << ref->fileName << "\n";
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
         m_loader->addJobIn(new LoadingJob(LOAD_JOB_MESH, newMeshRef));
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
            m_loader->addJobIn(new LoadingJob(LOAD_JOB_TEXTURE, newTextureRef));
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

void RenderingEngine::createShaders() {
   ShaderProgram newShader;
   string vertexShader, fragmentShader;
   
   //Vertex Lighting
   newShader.type = SHADER_VERTEX_LIGHT;
   newShader.numTextures = 1;
   vertexShader = m_modules->resourceManager->readShader("shaders/VertexLighting.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/VertexLighting.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), newShader);
   m_shaderPrograms[newShader.type] = newShader;
   
   //No Lighting
   newShader.type = SHADER_NO_LIGHT;
   newShader.numTextures = 1;
   vertexShader = m_modules->resourceManager->readShader("shaders/Simple.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/NoLighting.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), newShader);
   m_shaderPrograms[newShader.type] = newShader;
   
   //Bloom
   newShader.type = SHADER_BLOOM;
   newShader.numTextures = 1;
   vertexShader = m_modules->resourceManager->readShader("shaders/Simple.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/Bloom.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), newShader);
   m_shaderPrograms[newShader.type] = newShader;
   
   //Bloom Cull
   newShader.type = SHADER_BLOOM_CULL;
   newShader.numTextures = 1;
   vertexShader = m_modules->resourceManager->readShader("shaders/Simple.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/BloomCull.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), newShader);
   m_shaderPrograms[newShader.type] = newShader;
   
   //Combine
   newShader.type = SHADER_COMBINE;
   newShader.numTextures = 5;
   vertexShader = m_modules->resourceManager->readShader("shaders/Simple.vert");
   fragmentShader = m_modules->resourceManager->readShader("shaders/Add5.frag");
   newShader = buildProgram(vertexShader.c_str(), fragmentShader.c_str(), newShader);
   m_shaderPrograms[newShader.type] = newShader;
   
   m_curShaderProgram = NULL;
   setShaderProgram(SHADER_VERTEX_LIGHT);
}

void RenderingEngine::setShaderProgram(SHADER_TYPE type) {
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
   
   for (int i = 0; i < m_curShaderProgram->numTextures; i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      glUniform1i(m_curShaderProgram->uniforms.samplers[i], i);
   }
}

void RenderingEngine::createFrameBuffers() {
   FrameBuffer newFB;
   ivec2 size = m_screenSize;
   ivec2 loc(0, 0);
   vec4 clearColor(0.0, 0.0, 0.0, 1.0);
   
   // Create the defult screen buffer
   newFB.type = FRAME_BUFFER_SCREEN;
   newFB.clearColor = clearColor;
   newFB.bufferSize = size;
   newFB.viewSize = size;
   newFB.viewLoc = loc;
   newFB.fbo = 0;
   newFB.texture = 0;
   newFB.depth = 0;
   m_frameBuffers[newFB.type] = newFB;
   
   newFB.type = FRAME_BUFFER_PASS0;
   createFrameBuffer(newFB, true);
   m_frameBuffers[newFB.type] = newFB;
   
   newFB.type = FRAME_BUFFER_PASS1;
   createFrameBuffer(newFB, true);
   m_frameBuffers[newFB.type] = newFB;
   
   newFB.type = FRAME_BUFFER_REDUCE0;
   size = size / 2;
   newFB.bufferSize = size;
   newFB.viewSize = size;
   createFrameBuffer(newFB, true);
   m_frameBuffers[newFB.type] = newFB;
   
   newFB.type = FRAME_BUFFER_REDUCE1;
   size = size / 2;
   newFB.bufferSize = size;
   newFB.viewSize = size;
   createFrameBuffer(newFB, true);
   m_frameBuffers[newFB.type] = newFB;
   
   newFB.type = FRAME_BUFFER_REDUCE2;
   size = size / 2;
   newFB.bufferSize = size;
   newFB.viewSize = size;
   createFrameBuffer(newFB, true);
   m_frameBuffers[newFB.type] = newFB;
   
   newFB.type = FRAME_BUFFER_REDUCE3;
   size = size / 2;
   newFB.bufferSize = size;
   newFB.viewSize = size;
   createFrameBuffer(newFB, true);
   m_frameBuffers[newFB.type] = newFB;
   
   m_curFrameBuffer = NULL;
   setFrameBuffer(FRAME_BUFFER_SCREEN);
}

void RenderingEngine::setFrameBuffer(FRAME_BUFFER_TYPE type) {
   if (m_curFrameBuffer != NULL && type == m_curFrameBuffer->type) {
      return;
   }
   m_curFrameBuffer = &m_frameBuffers[type];
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_curFrameBuffer->fbo);
   glViewport(m_curFrameBuffer->viewLoc.x, m_curFrameBuffer->viewLoc.y, m_curFrameBuffer->viewSize.x, m_curFrameBuffer->viewSize.y);
}
