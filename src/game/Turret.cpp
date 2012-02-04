//
//  Turret.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include "Turret.h"

Turret::Turret(string headName, string headTexture, string midName, string midTexture, string footName, string footTexture, Modules *modules) 
: Object3d(), health(100)
{
   m_headMesh = new Mesh(headName, headTexture, modules);
   m_meshList.push_back(m_headMesh);
   
   m_midMesh = new Mesh(midName, midTexture, modules);
   m_meshList.push_back(m_midMesh);
   
   m_footMesh = new Mesh(footName, footTexture, modules);
   m_meshList.push_back(m_footMesh);
   
   mat4 modelMtx = mat4::Translate(0, 0, -100);
   modelMtx = mat4::Rotate(90, vec3(0, 1, 0)) * modelMtx;
   
   m_headMesh->setModelMtx(modelMtx);
   m_midMesh->setModelMtx(modelMtx);
   m_footMesh->setModelMtx(modelMtx);
}

Turret::~Turret()
{
   
}
