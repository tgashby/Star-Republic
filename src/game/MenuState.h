//
//  MenuState.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_MenuState_h
#define StarRepublic_MenuState_h

#include "State.h"

/**
 * MenuState is the Menu state, before the game starts
 */
class MenuState : public State 
{
public:
   void initialize();
   
   void display();
   
   void cleanUp();
   
private:
   
};

#endif
