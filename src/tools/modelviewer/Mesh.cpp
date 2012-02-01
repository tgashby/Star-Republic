#include <iostream>
#include "Meshes.h"

Mesh::Mesh(string meshName, string textureName) {
   m_meshName = meshName;
   m_textureName = textureName;
   meshRef = MeshRef();
   meshMtx = mat4::Identity();
   color = vec4(0.8, 0.8, 0.8, 1.0);
   normalType = LOAD_NORMAL_VERTEX;
   size = 2;
}

Mesh::~Mesh() {
   
}

string Mesh::getMeshName() {
   return m_meshName;
}

string Mesh::getTextureName() {
   return m_textureName;
}

MeshData* Mesh::getMeshData() {
   /*
   float verts[] = {
      -1.0, -1.0, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0, 0.0,   0.0, 0.0,
      -1.0, 1.0, 0.0,    0.0, 0.0, 1.0,   0.0, 0.0, 0.0,   1.0, 0.0,
      1.0, 1.0, 0.0,     0.0, 0.0, 1.0,   0.0, 0.0, 0.0,   1.0, 1.0,
      1.0, -1.0, 0.0,    0.0, 0.0, 1.0,   0.0, 0.0, 0.0,   0.0, 1.0
   };
   
   MeshData *meshData = new MeshData();
   
   meshData->vertexCount = 4;
   meshData->vertices = new float[44];
   for (int i = 0; i < 44; i++) {
      meshData->vertices[i] = verts[i];
      if ((i % 11) <= 1 ) {
         meshData->vertices[i] = 6.0 * meshData->vertices[i];
      }
   }
   
   meshData->indexCount = 6;
   meshData->indices = new unsigned short[6];
   meshData->indices[0] = 0;
   meshData->indices[1] = 1;
   meshData->indices[2] = 2;
   meshData->indices[3] = 2;
   meshData->indices[4] = 3;
   meshData->indices[5] = 0;
   
   return meshData;*/
   return NULL;
}

ImageData* Mesh::getImageData() {
   return NULL;
}