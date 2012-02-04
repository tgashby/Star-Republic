#include "RenderingEngine.h"

#define STRINGIFY(A)  #A
#include "TexturedLighting.vert"
#include "TexturedLighting.frag"

RenderingEngine::RenderingEngine(Modules *modules) {
   m_modules = modules;
   m_meshList = list<MeshRef>(0);
   m_textureList = list<TextureRef>(0);
   //NOTE: WAS EQUAL TO NULL. SET TO SOMETHING USELESS SO CODE WOULD STOP SEGFAULTING
   m_camera = NULL;
   
#ifdef _WIN32
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }
#endif

   // Create the GLSL program.
   GLuint program = buildProgram(SimpleVertexShader, SimpleFragmentShader);
   glUseProgram(program);
   
   // Extract the handles to attributes and uniforms.
   m_attributes.position = glGetAttribLocation(program, "Position");
   m_attributes.normal = glGetAttribLocation(program, "Normal");
   m_attributes.diffuseMaterial = glGetAttribLocation(program, "DiffuseMaterial");
   m_attributes.textureCoord = glGetAttribLocation(program, "TextureCoord");
   m_uniforms.projection = glGetUniformLocation(program, "Projection");
   m_uniforms.modelview = glGetUniformLocation(program, "Modelview");
   m_uniforms.normalMatrix = glGetUniformLocation(program, "NormalMatrix");
   m_uniforms.lightPosition = glGetUniformLocation(program, "LightPosition");
   m_uniforms.ambientMaterial = glGetUniformLocation(program, "AmbientMaterial");
   m_uniforms.specularMaterial = glGetUniformLocation(program, "SpecularMaterial");
   m_uniforms.shininess = glGetUniformLocation(program, "Shininess"); 
   m_uniforms.sampler = glGetUniformLocation(program, "Sampler");
   
   // set a texture
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(m_uniforms.sampler, 0);
	
   // Set up some default material parameters.
   glUniform3f(m_uniforms.ambientMaterial, 0.4f, 0.4f, 0.4f);
   glUniform3f(m_uniforms.specularMaterial, 0.5, 0.5, 0.5);
   glUniform1f(m_uniforms.shininess, 50);
   
   // Initialize various state.
   glEnableVertexAttribArray(m_attributes.position);
   glEnableVertexAttribArray(m_attributes.normal);
   glEnableVertexAttribArray(m_attributes.textureCoord);
   glEnable(GL_DEPTH_TEST);
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
   //m_objectList.push_back(obj);
}


void RenderingEngine::removeObject3d(IObject3d *obj) {
   // get meshes from the new object
   list<IMesh *> *objMeshes = obj->getMeshes();
   list<IMesh *>::iterator mesh;
   for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
      unLoadMesh(*mesh);
   }
   // remove the object from objectList
}


void RenderingEngine::render(list<IObject3d *> &objects) {
   glClearColor(0,0,0,0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   
   // Get the projection * view matrix from the camera.
   mat4 projectionViewMatrix = m_camera->getProjectionViewMtx();
   glUniformMatrix4fv(m_uniforms.projection, 1, 0, projectionViewMatrix.Pointer());
   
   // Set the light position.
   vec4 lightPosition(1, 1, 1, 0);
   glUniform3fv(m_uniforms.lightPosition, 1, lightPosition.Pointer());
   
   list<IObject3d *>::iterator obj;
   list<IMesh *> *objMeshes;
   list<IMesh *>::iterator mesh;
   
   for (obj = objects.begin(); obj != objects.end(); ++obj) {
      objMeshes = (*obj)->getMeshes();
      for (mesh = objMeshes->begin(); mesh != objMeshes->end(); ++mesh) {
         // Skip the mesh if it is not visible.
         if (!(*mesh)->isVisible())
            continue;
         
         // Set the model view matrix
         mat4 modelMtx = (*mesh)->getModelMtx() ;
         glUniformMatrix4fv(m_uniforms.modelview, 1, 0, modelMtx.Pointer());
         
         // Set the normal matrix
         mat3 normalMtx = modelMtx.ToMat3();
         glUniformMatrix3fv(m_uniforms.normalMatrix, 1, 0, normalMtx.Pointer());
         
         // Set the diffuse color.
         vec4 color =(*mesh)->getColor();
         glVertexAttrib4f(m_attributes.diffuseMaterial, color.x, color.y, color.z, color.w);
         
         // Draw the surface.
         MeshRef meshRef = (*mesh)->getMeshRef();
         TextureRef textureRef = (*mesh)->getTextureRef();
         int stride = 11 * sizeof(GLfloat);
         const GLvoid* normalOffset = (const GLvoid*) (3 * sizeof(GLfloat));
         const GLvoid* texCoordOffset = (const GLvoid*) (3 * sizeof(vec3));
         GLint position = m_attributes.position;
         GLint normal = m_attributes.normal;
         GLint texCoord = m_attributes.textureCoord;
         
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

GLuint RenderingEngine::buildProgram(const char* vertexShaderSource,
                                     const char* fragmentShaderSource) const {
   GLuint vertexShader = buildShader(vertexShaderSource, GL_VERTEX_SHADER);
   GLuint fragmentShader = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
   
   GLuint programHandle = glCreateProgram();
   glAttachShader(programHandle, vertexShader);
   glAttachShader(programHandle, fragmentShader);
   glLinkProgram(programHandle);
   
   GLint linkSuccess;
   glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
   if (linkSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
      std::cout << messages;
      exit(1);
   }
   
   return programHandle;
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
      newMeshRef = *meshRef;
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
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData->size.x, textureData->size.y, 0,
                   GL_BGRA, GL_UNSIGNED_BYTE, textureData->pixels);
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
