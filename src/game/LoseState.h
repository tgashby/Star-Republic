//
//  LoseState.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_LoseState_h
#define StarRepublic_LoseState_h
#include "State.h"

/**
 * LoseState is the losing state
 */
class LoseState : public State 
{
public:
   void initialize();
   
   void display();
   
   void cleanUp();
   
private:
   
};

#endif
