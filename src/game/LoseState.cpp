//
//  LoseState.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "LoseState.h"

void LoseState::initialize()
{
   // Initialize screen
   
   m_initialized = true;
}

void LoseState::display()
{
   if (!m_initialized) 
   {
      std::cerr << "LoseState not initialized before display!\n";
      return;
   }
   
   // Display the Lose
}

void LoseState::cleanUp()
{
   if (!m_initialized)
   {
      std::cerr << "LoseState not initialzied before cleanUp!\n";
      return;
   }
   
   // Clean it up
}