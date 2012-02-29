/**
 * SoundManager - A class to play sound effects and music
 */

#ifndef StarRepub_StoundManager_h
#define StarRepub_StoundManager_h

#include "Sound.h"
#include "Interfaces.h"
#include "SDL_include.h"

using namespace std;

/**
 * SoundManager Class, deals with playing various sounds and background music
 */
class SoundManager : public ISoundManager {
public:
   /** 
    * Constructor - loads sounds and initalizes music streams 
    */ 
   SoundManager();

   /**
    * Destructor - Removes all loaded sounds and shuts down 
    * music streams. 
    */
   ~SoundManager();
   
   /**
    * playSound - Plays a sound that the SoundManager has loaded. This
    * will play a random sound assoicated with this event once. 
    */
   void playSound(SoundEvent event);
   
private:
   vector<Sound*>  m_sounds[numSoundEvents]; 
    
};

#endif
