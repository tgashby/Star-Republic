//
//  Turret.cpp
//  476 Game
//
//  Created by Taggart Ashby on 1/22/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include "Turret.h"

void Turret::Turret(Vector3<float> position, Vector3<float> direction, 
                    float velocity, BoundingStructure bounds, BoundingStructre range) 
: GameObject(position, direction, velocity, bounds), BoundingSphere(range)
{
   firing = false;
}


void Turret::tic( int dt )
{
	GameObject::tic(dt);

	if (!firing || checkRange(player))
	{
		fire();
	}

	for (std::list<Bullet>::iterator i = bullets.begin(); i < bullets.end(); i++)
	{
		i->tic(dt);
	}	
}


void Turret::draw()
{
   GameObject::draw();
}


bool Turret::checkRange()
{
	Vector3<float> playerPos = player.getPosition();

   double sqr_dist = 
      (position.x - playerPos.x) * (position.x - playerPos.x) + 
      (position.y - playerPos.y) * (position.y - playerPos.y) +
      (position.z - playerPos.z) * (position.x - playerPos.z);

   // Square root is expensive, just compare the squares
   return sqr_dist < range.getRadius() * range.getRadius();  
}

void Turret::doCollision( GameObject & other )
{
	const type_info otherId = typeid(other);

	if (otherId == typeid(Player))
	{
		// Asplode
	} 
	else if (otherId == typeid(Bullet)
	{
		// Still asplode
	}	
}

void Turret::fire()
{
	Vector3<float> playerPos = player.getPostion();

	Vector3<float> bulletDir = playerPos - position;

	bulletDir.Normalize();

	bullets.push_back(Bullet(position, bulletDir, 10f, BoundingSphere(0.5f));
}

Model Turret::getModel()
{
	GameObject::getModel();
}

BoundingObject Turret::getBoundingObject()
{
	GameObject::getBoundingObject();
}
