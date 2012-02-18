//
//  WorldGrid.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "WorldGrid.h"

WorldGrid::WorldGrid(WorldData& world)
   : m_world(world)
{
   makeGrid();
   placeTurrets();
}

WorldGrid::~WorldGrid()
{
   
}

void WorldGrid::makeGrid()
{
   // Connect the points into quadrants
   for (std::vector<vec3>::iterator i = m_world.path.begin(); i != m_world.path.end(); i += 2) 
   {
      m_quadrants.push_back(Quadrant(*i, *(i+1)));
   }
}