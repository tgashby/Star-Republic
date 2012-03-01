#include "SoundManager.h"
#include "ResourceManager.h"


SoundManager::SoundManager(){ 
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon1.wav"));
   m_sounds[PlayerMissile].push_back(loadSound("sound/missileLaunch.wav"));
}


SoundManager::~SoundManager(){
   for(int ii; ii < numSoundEvents; ii++){
      m_sounds[ii].clear();
   }
}

void SoundManager::playSound(SoundEvent event){
   if(m_sounds[event].size() != 0){
      m_sounds[event][0]->play(0);     
      if(event == PlayerGun){
         //cerr << "This totally ran? Size is?"<<m_sounds[event].size() << "\n";
      }
   }

}

