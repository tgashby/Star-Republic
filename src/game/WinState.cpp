//
//  WinState.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "WinState.h"

void WinState::initialize()
{
   // Initialize screen
   
   m_initialized = true;
}

void WinState::display()
{
   if (!m_initialized) 
   {
      std::cerr << "WinState not initialized before display!\n";
      return;
   }
   
   // Display the Win
}

void WinState::cleanUp()
{
   if (!m_initialized)
   {
      std::cerr << "WinState not initialzied before cleanUp!\n";
      return;
   }
   
   // Clean it up
}