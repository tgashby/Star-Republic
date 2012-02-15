//
//  WinState.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_WinState_h
#define StarRepublic_WinState_h
#include "State.h"

/**
 * WinState is the winning state
 */
class WinState : public State 
{
public:
   void initialize();
   
   void display();
   
   void cleanUp();
   
private:
   
};

#endif
