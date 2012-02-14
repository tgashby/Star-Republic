//
//  Flyer.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Flyer_h
#define StarRepub_Flyer_h

#include <cassert>

#include "../engine/Interfaces.h"
#include "GameObject.h"

class Flyer : public GameObject
{
public:
   // Constructors
   Flyer(Vector3<float> progressVec, Vector3<float> progressVelocityVec,
         Vector3<float> currentHeadPosVec, Vector3<float> previousHeadPosVec,
         Vector3<float> currentHeadUpVec, Vector3<float> previousHeadUpVec);
   Flyer();
   
   
   // Destructor
   ~Flyer();
   
   // Update
   void tic(uint64_t dt);
   
   // Setters
   void setProgressVelocity(Vector3<float> vel);
   void setBearing(Vector3<float> headPos, Vector3<float> headUp);
   void setProgress(Vector3<float> pos);
   void setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp,
                 Vector3<float> prevHeadPos, Vector3<float> prevHeadUp);
   
   // Getters
   Vector3<float> getProgress();
   Vector3<float> getProgressVelocity();
   Vector3<float> getForward();
   
protected:
   Vector3<float> m_progressVelocity;
   Vector3<float> m_progress;
   Vector3<float> m_currentHeadPos;
   Vector3<float> m_previousHeadPos;
   Vector3<float> m_currentHeadUp;
   Vector3<float> m_previousHeadUp;
};


#endif
