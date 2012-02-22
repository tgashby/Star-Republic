//
//  Enemy.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Enemy_h
#define StarRepub_Enemy_h

#include "Player.h"

/**
 * Enemy Interface, used only to force enemies to have a player reference
 */
class Enemy 
{
public:
   /**
    * Enemy constructor, set the enemy's player reference to the given player
    * reference
    * @param player the player reference
    */
   Enemy(Player& player);
   
   void setPlayer(Player player);
   
protected:
   Player& m_playerRef;
};

#endif
