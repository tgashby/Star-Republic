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
   m_chunk = NULL;
   m_music = NULL;
}

void Sound::setChunk(Mix_Chunk* chunk)
{
   m_chunk = chunk;
}

void Sound::setMusic(Mix_Music* music)
{
   m_music = music;
}

void Sound::play(int loops)
{
   if (m_chunk != NULL) 
   {
      m_channel = Mix_PlayChannel(-1, m_chunk, loops);
   }
   else
   {
      m_channel = Mix_PlayMusic(m_music, loops);
   }
   
   if (m_channel == -1)
   {
      printf("Mix_Play error: %s\n", Mix_GetError());
   }
}

void Sound::pause()
{
   if (m_chunk != NULL && m_channel != INVALID_CHANNEL) 
   {
      Mix_Pause(m_channel);
   }
   else
   {
      Mix_PauseMusic();
   }
}

void Sound::resume()
{
   if (m_chunk != NULL && m_channel != INVALID_CHANNEL) 
   {
      Mix_Resume(m_channel);
   }
   else
   {
      Mix_ResumeMusic();
   }
}