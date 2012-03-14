//
//  PathObject.cpp
//  StarRepub
//
//  Created by Robert Crosby on 3/13/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#include "PathObject.h"

PathObject::PathObject(string fileName, string textureName, Modules *modules,
                       vec3 pos, vec3 forw, vec3 up) {
   m_up = up;
   m_position = pos;
   m_forward = forw;
   
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);
   
   m_mesh->setScale(1.0);
   m_mesh->setModelMtx(mat4::Magic(m_forward, m_up, m_position));
   m_mesh->setShaderType(SHADER_BLOOM);
   
   m_textureOffset = 0.0;
}

PathObject::~PathObject() {
   
}

void PathObject::tic(uint64_t time) {
   // move the texture
   m_textureOffset += time * TEXTURE_MOVE_RATE;
   if (m_textureOffset > 1.0) {
      m_textureOffset -= 1.0;
   }
   m_mesh->setTextureMtx(mat4::Translate(-m_textureOffset, 0, 0));
}

void PathObject::doCollision(GameObject & other) {
   // do nothing
}

vec3 PathObject::getPosition() {
   return m_position;
}