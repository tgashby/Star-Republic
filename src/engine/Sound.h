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

/** 
 * Sound Class, for playing sounds and music.
 * Sound has two fields, but should only ever have one
 * that isn't NULL. You should set either the chunck field OR the
 * music field, but never both.
 */
class Sound : public ISound 
{
public:
   /**
    * Sound constructor, initialized chunk and music to NULL
    */
   Sound();
   
   /**
    * setChunck sets the chunck field to a given sound.
    * @param chunk the chunk to set this sound's chunk field to
    */
   void setChunk(Mix_Chunk* chunk);
   
   /**
    * setChunck sets the music field to a given track.
    * @param music the music to set this sound's music field to
    */
   void setMusic(Mix_Music* music);
   
   /**
    * play plays the given sound
    * @param loops the number of times to play the sound, -1 is play infinitely.
    */
   void play(int loops);
   
   /**
    * pause pauses the given sound
    */
   void pause();
   
   /**
    * resume resumes the given sound
    */
   void resume();
   
private:
};

#endif
