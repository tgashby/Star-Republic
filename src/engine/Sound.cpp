//
//  MusicTrack.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "Sound.h"

Sound::Sound()
{
   chunk = NULL;
   music = NULL;
}

void Sound::setChunk(Mix_Chunk* chunk)
{
   this->chunk = chunk;
}

void Sound::setMusic(Mix_Music* music)
{
   this->music = music;
}

void Sound::play(int loops)
{
   if (this->chunk != NULL) 
   {
      channel = Mix_PlayChannel(-1, this->chunk, loops);
   }
   else
   {
      channel = Mix_PlayMusic(this->music, loops);
   }
   
   if (channel == -1)
   {
      printf("Mix_Play error: %s\n", Mix_GetError());
   }
}

void Sound::pause()
{
   if (this->chunk != NULL && channel != INVALID_CHANNEL) 
   {
      Mix_Pause(channel);
   }
   else
   {
      Mix_PauseMusic();
   }
}

void Sound::resume()
{
   if (this->chunk != NULL && channel != INVALID_CHANNEL) 
   {
      Mix_Resume(channel);
   }
   else
   {
      Mix_ResumeMusic();
   }
}