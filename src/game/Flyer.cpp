//
//  Flyer.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "Flyer.h"

Flyer::Flyer(Vector3<float> progressVec, Vector3<float> progressVelocityVec,
             Vector3<float> currentHeadPosVec, Vector3<float> previousHeadPosVec,
             Vector3<float> currentHeadUpVec, Vector3<float> previousHeadUpVec)
{
   m_progressVelocity = progressVec;
   m_progress = progressVec;
   m_currentHeadPos = currentHeadPosVec;
   m_previousHeadPos = previousHeadPosVec;
   m_currentHeadUp = currentHeadUpVec;
   m_previousHeadUp = previousHeadUpVec;
}

Flyer::Flyer()
{
   m_progressVelocity = vec3(0,0,0);
   m_progress = vec3(0,0,0);
   m_currentHeadPos = vec3(0,0,0);
   m_previousHeadPos = vec3(0,0,0);
   m_currentHeadUp = vec3(0,0,0);
   m_previousHeadUp = vec3(0,0,0);
}

Flyer::~Flyer()
{
   
}

void Flyer::tic(uint64_t dt)
{
   
}

void Flyer::setProgressVelocity(Vector3<float> vel)
{
   m_progressVelocity = vel;
}

void Flyer::setBearing(Vector3<float> headPos, Vector3<float> headUp)
{
   m_currentHeadUp = headPos;
   m_currentHeadUp = headUp;
}

void Flyer::setProgress(Vector3<float> pos)
{
   m_progress = pos;
}

void Flyer::setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp,
              Vector3<float> prevHeadPos, Vector3<float> prevHeadUp)
{
      m_up = prevHeadUp;
      m_previousHeadUp = prevHeadUp;
      m_previousHeadPos = prevHeadPos;
      m_currentHeadUp = currHeadUp;
      m_currentHeadPos = currHeadPos;
}

Vector3<float> Flyer::getProgress()
{
   return m_progress;
}

Vector3<float> Flyer::getProgressVelocity()
{
   assert(m_progressVelocity.x == m_progressVelocity.x);
   assert(m_progressVelocity.y == m_progressVelocity.y);
   assert(m_progressVelocity.z == m_progressVelocity.z);
   return m_progressVelocity;
}

Vector3<float> Flyer::getForward()
{
   return (m_currentHeadPos - m_progress).Normalized();
}
