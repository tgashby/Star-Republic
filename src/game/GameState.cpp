//
//  GameState.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "GameState.h"

void GameState::initialize()
{
   // Initialize screen
   
   m_initialized = true;
}

void GameState::display()
{
   if (!m_initialized) 
   {
      std::cerr << "GameState not initialized before display!\n";
      return;
   }
   
   // Display the Game
}

void GameState::cleanUp()
{
   if (!m_initialized)
   {
      std::cerr << "GameState not initialzied before cleanUp!\n";
      return;
   }
   
   // Clean it up
}