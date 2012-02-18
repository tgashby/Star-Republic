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

/**
 * Flyer is an interface for flying enemies and objects
 */ 
class Flyer : public GameObject
{
public:
   /**
    * Flyer construction to set all the fields of a flyer manually
    * @param progressVec the progress on the path
    * @param progressVelocityVec the progress velocity on the path
    * @param currentHeadPosVec the current head position
    * @param previousHeadPosVec the previous head position
    * @param currentHeadUpVec the current head position's up vector
    * @param previousHeadUpVec the previous head position's up vector
    */
   Flyer(Vector3<float> progressVec, Vector3<float> progressVelocityVec,
         Vector3<float> currentHeadPosVec, Vector3<float> previousHeadPosVec,
         Vector3<float> currentHeadUpVec, Vector3<float> previousHeadUpVec);
   
   /**
    * Flyer constructor that sets everything to default values
    */
   Flyer();
   
   
   // Destructor
   ~Flyer();
   
   /**
    * tic updates the flying object based on time passed, current each
    * subclass overwrites this
    */
   void tic(uint64_t dt);
   
   /**
    * setProgressVelocity sets the progress velocity
    * @param vel the new progress velocity
    */
   void setProgressVelocity(Vector3<float> vel);
   
   /**
    * setBearing sets the flyer's heading
    * @param headPos the new heading position
    * @param headUp the new heading's up vector
    */ 
   void setBearing(Vector3<float> headPos, Vector3<float> headUp);
   
   /**
    * setProgress sets the progress position along the path
    * @param pos the new position
    */
   void setProgress(Vector3<float> pos);
   
   /**
    * setHeads sets the current and previous headings
    * @param currHeadPos the new current heading position
    * @param currHeadUp the new current heading's up vector
    * @param prevHeadPos the new previous heading position
    * @param prevHeadUp the new previous heading's up vector
    */
   void setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp,
                 Vector3<float> prevHeadPos, Vector3<float> prevHeadUp);
   
   /**
    * getProgress returns the current position along the path
    */
   Vector3<float> getProgress();
   
   /**
    * getProgressVelocity returns the speed at which you're moving along the
    * path
    */
   Vector3<float> getProgressVelocity();
   
   /**
    * getForward returns the flyer's forward vector
    */
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
