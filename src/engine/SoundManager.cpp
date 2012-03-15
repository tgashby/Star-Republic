#include "SoundManager.h"
#include "ResourceManager.h"


SoundManager::SoundManager(){ 
   initSound();
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon3.wav"));
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon4.wav"));
   m_sounds[PlayerGun].push_back(loadSound("sound/weapon5.wav"));
   m_sounds[PlayerShotgun].push_back(loadSound("sound/shotgun1.wav"));
   m_sounds[PlayerMissile].push_back(loadSound("sound/missileLaunch.wav"));
   m_sounds[PlayerHit].push_back(loadSound("sound/hitsound1.wav"));
   m_sounds[PlayerHit].push_back(loadSound("sound/hitsound2.wav"));
   m_sounds[PlayerHit].push_back(loadSound("sound/hitsound3.wav"));
   m_sounds[Explosion].push_back(loadSound("sound/explosion1.wav"));
   m_sounds[Explosion].push_back(loadSound("sound/explosion2.wav"));
   m_sounds[Explosion].push_back(loadSound("sound/explosion3.wav"));
   m_sounds[Explosion].push_back(loadSound("sound/explosion6.wav"));
   m_sounds[Explosion].push_back(loadSound("sound/explosion7.wav"));
   
   m_backgroundSounds[Boost] = loadSound("sound/boost.wav");
   m_backgroundPlaying[Boost] = false;
   m_backgroundSounds[Idle] = loadSound("sound/ambient1.wav");
   m_backgroundPlaying[Idle] = false;

   m_music.push_back(loadMusic("music/music1.ogg"));
   m_music.push_back(loadMusic("music/music2.ogg"));

   m_currentTrack = m_music.begin();

   m_delayCountdown = defaultDelay;
   m_musicPlaying = false;
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
   if(!m_backgroundPlaying[sound]){
      m_backgroundPlaying[sound] = true;
      m_backgroundSounds[sound]->play(-1);
   }
   else {
      cerr << "Playing something that's already running";
   }
} 

void SoundManager::stopBackgroundSound(BackgroundSound sound){
    if(m_backgroundPlaying[sound]){
      m_backgroundPlaying[sound] = false;
      m_backgroundSounds[sound]->stop();
   }  
}

void SoundManager::startMusic(){
   if(!m_musicPlaying){
      m_musicPlaying = true;
      playNextTrack(); 
   }

}

void SoundManager::stopMusic(){

   m_musicPlaying = true;
   (*m_currentTrack)->stop();
}

void SoundManager::playNextTrack(){
   if(m_currentTrack++ >= m_music.end()-1){
      m_currentTrack = m_music.begin();
   }
   else{
      m_currentTrack = m_currentTrack++;
   }
   
   (*m_currentTrack)->play(0);
}

void SoundManager::tic(uint64_t dt){
   if(!((*m_currentTrack)->isPlaying())){
      if(!m_countdownRunning){
         m_countdownRunning = true;   
      } 
      else{
         m_delayCountdown -= dt;
      }
   }
   if(m_delayCountdown < 0){
      playNextTrack();
      m_delayCountdown = defaultDelay;
      m_countdownRunning = false;   
   } 

}
