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
   m_meshList = list<MeshRef>(0);
   m_textureList = list<TextureRef>(0);
   m_camera = NULL;
   TTF_Init();
   
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
   SDL_Color color2 = {0, 0, 0};
   
   
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
         // Skip the mesh if it is not visible.
         if (!(*mesh)->isVisible())
            continue;
         
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
         MeshRef meshRef = (*mesh)->getMeshRef();
         TextureRef textureRef = (*mesh)->getTextureRef();
         int stride = 11 * sizeof(GLfloat);
         const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
         const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
         GLint position = m_curShaderProgram->attributes.position;
         GLint normal = m_curShaderProgram->attributes.normal;
         GLint texCoord = m_curShaderProgram->attributes.textureCoord;
         
         glBindTexture(GL_TEXTURE_2D, textureRef.textureBuffer);
         glBindBuffer(GL_ARRAY_BUFFER, meshRef.vertexBuffer);
         glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, 0);
         glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, stride, normalOffset);
         glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, stride, texCoordOffset);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRef.indexBuffer);
         glDrawElements(GL_TRIANGLES, meshRef.indexCount, GL_UNSIGNED_SHORT, 0);
      }
   }
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
   // Get the MeshRef and TextureRef from the new mesh.
   MeshRef newMeshRef = newMesh->getMeshRef();
   TextureRef newTextureRef = newMesh->getTextureRef();
   
   // Check if the mesh was already loaded.
   //list<MeshRef>::iterator meshRef = find(m_meshList.begin(), m_meshList.end(), newMeshRef);
   list<MeshRef>::iterator meshRef = m_meshList.begin();
   while (meshRef != m_meshList.end() && !(*meshRef == newMeshRef))
      ++meshRef;

   if (meshRef != m_meshList.end()) {
      newMesh->setMeshRef(*meshRef);
      meshRef->count += 1;
   }
   else {
      // Get the meshData.
      MeshData *meshData = newMesh->getMeshData();
      
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
      
      // Setup a new mesh reference for the render engine
      newMeshRef.name = newMesh->getMeshName();
      newMeshRef.vertexBuffer = vertexBuffer;
      newMeshRef.indexBuffer = indexBuffer;
      newMeshRef.indexCount = meshData->indexCount;
      newMeshRef.bounds = meshData->bounds;
      newMesh->setMeshRef(newMeshRef);
      m_meshList.push_back(newMeshRef);
      
      // Clean up the mesh data
      delete[] meshData->vertices;
      delete[] meshData->indices;
      delete meshData;
   }
   
   TextureRef newTextrueRef = newMesh->getTextureRef();
   
   //list<TextureRef>::iterator textureRef = std::find(m_textureList.begin(), m_textureList.end(), newTextrueRef);
   list<TextureRef>::iterator textureRef = m_textureList.begin();
   while (textureRef != m_textureList.end() && !(*textureRef == newTextureRef))
      ++textureRef;

   if (textureRef != m_textureList.end()) {
      newMesh->setTextureRef(*textureRef);
      textureRef->count += 1;
   }
   else {
      // Get the texture
      TextureData *textureData = newMesh->getTextureData();
      
      // Load a new texture.
      GLuint textureBuffer;
      glGenTextures(1, &textureBuffer);
      glBindTexture(GL_TEXTURE_2D, textureBuffer);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->size.x, textureData->size.y, 0,
                   GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);
      delete textureData;
      glGenerateMipmapEXT(GL_TEXTURE_2D);
      
      // Setup a new texture reference
      newTextrueRef.name = newMesh->getTextureName();
      newTextrueRef.textureBuffer = textureBuffer;
      newMesh->setTextureRef(newTextrueRef);
      m_textureList.push_back(newTextrueRef);
   }
   
}

void RenderingEngine::unLoadMesh(IMesh* rmvMesh) {
   MeshRef findMeshRef = rmvMesh->getMeshRef();
   //list<MeshRef>::iterator meshRef = find(m_meshList.begin(), m_meshList.end(), findMeshRef);
   list<MeshRef>::iterator meshRef = m_meshList.begin();
   while (meshRef != m_meshList.end() && !(*meshRef == findMeshRef))
      ++meshRef;
   if (meshRef != m_meshList.end()) {
      meshRef->count -= 1;
      if (meshRef->count < 1) {
         // remove the VBOs
         glDeleteBuffers(1, &meshRef->vertexBuffer);
         glDeleteBuffers(1, &meshRef->indexBuffer);
         m_meshList.erase(meshRef);
      }
   }
   
   TextureRef findTextrueRef = rmvMesh->getTextureRef();
   //list<TextureRef>::iterator textureRef = std::find(m_textureList.begin(), m_textureList.end(), findTextrueRef);
   list<TextureRef>::iterator textureRef = m_textureList.begin();
   while (textureRef != m_textureList.end() && !(*textureRef == findTextrueRef))
      ++textureRef;
   if (textureRef != m_textureList.end()) {
      textureRef->count -= 1;
      if (textureRef->count < 1) {
         // remove the texture
         glDeleteTextures(1, &textureRef->textureBuffer);
         m_textureList.erase(textureRef);
      }
   }
}
