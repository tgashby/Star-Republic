//
//  MusicTrack.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_MusicTrack_h
#define StarRepub_MusicTrack_h

#include "Interfaces.h"
#include "SDL_include.h"

class Sound : public ISound 
{
public:
   Sound();
   
   void setChunk(Mix_Chunk*);
   void setMusic(Mix_Music*);
   
   void play(int loops);
   void pause();
   void resume();
   
private:
};

#endif
