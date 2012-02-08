//
//  Enemy.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Enemy_h
#define StarRepub_Enemy_h

#include "GameObject.h"
#include "Player.h"

class Enemy : public GameObject 
{
public:
   Enemy(Player& player);
   
protected:
   Player& m_playerRef;
};

#endif
