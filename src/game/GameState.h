//
//  GameState.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_GameState_h
#define StarRepublic_GameState_h
#include "State.h"

/**
 * GameState is the main game state
 */
class GameState : public State 
{
public:
   void initialize();
   
   void display();
   
   void cleanUp();
   
private:
   
};

#endif
