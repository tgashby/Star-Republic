#include "SoundManager.h"
#include "ResourceManager.h"


SoundManager::SoundManager(){ 
   cerr << "Loading Sound Manager \n";
   initSound();
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon3.wav"));
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon4.wav"));
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon5.wav"));
   m_sounds[PlayerMissile].push_back(loadSound("sound/missileLaunch.wav"));
   
   //m_backgroundSounds[Boost] = loadSound("sound/boost.wav");
   //m_backgroundSounds[Idle] = loadSound("sound/ambient1.wav");
}


SoundManager::~SoundManager(){
   for(int ii = 0; ii < numSoundEvents; ii++){
      m_sounds[ii].clear();
   }
}

void SoundManager::playSound(SoundEvent event){
   int soundIdx;
   if(m_sounds[event].size() != 0){
      soundIdx = rand() % m_sounds[event].size();
      m_sounds[event][soundIdx]->play(0);     
   }

}

void SoundManager::playBackgroundSound(BackgroundSound sound){
//   if(!m_backgroundPlaying[sound]){
 //     m_backgroundPlaying[sound] = true;
//      m_backgroundSounds[sound]->play(-1);
//   }
} 

void SoundManager::stopBackgroundSound(BackgroundSound sound){
   
}
