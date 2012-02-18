//
//  MenuState.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "MenuState.h"

void MenuState::initialize()
{
   // Initialize screen
   
   m_initialized = true;
}

void MenuState::display()
{
   if (!m_initialized) 
   {
      std::cerr << "MenuState not initialized before display!\n";
      return;
   }
   
   // Display the menu
}

void MenuState::cleanUp()
{
   if (!m_initialized)
   {
      std::cerr << "MenuState not initialzied before cleanUp!\n";
      return;
   }
   
   // Clean it up
}