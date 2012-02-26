//
//  TestWorldGrid.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/25/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include <UnitTest++/UnitTest++.h>
#include "WorldGrid.h"
#include "ResourceManager.h"

struct SimpleWorldGridFixture
{
   SimpleWorldGridFixture()
   {
      m_modules = new Modules();
      m_modules->resourceManager = new ResourceManager();
      m_worldData = m_modules->resourceManager->readWorldData("test/testCourse.wf");
      m_worldGrid = new WorldGrid(*m_worldData, m_modules);
   }
   
   ~SimpleWorldGridFixture()
   {
      delete m_modules;
      delete m_worldData;
      delete m_worldGrid;
   }
   
   WorldGrid* m_worldGrid;
   WorldData* m_worldData;
   Modules* m_modules;
   static int quadrants, units, obj3Ds;
};
int SimpleWorldGridFixture::quadrants = 3;
int SimpleWorldGridFixture::units = 1;
int SimpleWorldGridFixture::obj3Ds = 1;

struct ComplexWorldGridFixture
{
   ComplexWorldGridFixture()
   {
      m_modules = new Modules();
      m_modules->resourceManager = new ResourceManager();
      m_worldData = m_modules->resourceManager->readWorldData("test/testCourse2.wf");
      m_worldGrid = new WorldGrid(*m_worldData, m_modules);
   }
   
   ~ComplexWorldGridFixture()
   {
      delete m_modules;
      delete m_worldData;
      delete m_worldGrid;
   }
   
   WorldGrid* m_worldGrid;
   WorldData* m_worldData;
   Modules* m_modules;
   static int quadrants, units, obj3Ds;
};
int ComplexWorldGridFixture::quadrants = 5;
int ComplexWorldGridFixture::units = 5;
int ComplexWorldGridFixture::obj3Ds = 8;

typedef SimpleWorldGridFixture swgf;
typedef ComplexWorldGridFixture cwgf;

/*
 WorldGrid()/makeGrid() tests
*/
TEST_FIXTURE(swgf, simpleInitQuadCount)
{
   CHECK_EQUAL(SimpleWorldGridFixture::quadrants, m_worldGrid->m_quadrants.size());
}

TEST_FIXTURE(swgf, simpleInitStartEndLocs)
{
   // Starts
   CHECK_EQUAL(vec3(0,0,0), m_worldGrid->m_quadrants.at(0)->m_startPt.loc);
   CHECK_EQUAL(vec3(500,0,0), m_worldGrid->m_quadrants.at(1)->m_startPt.loc);
   CHECK_EQUAL(vec3(1000,0,0), m_worldGrid->m_quadrants.at(2)->m_startPt.loc);
   
   // Ends
   CHECK_EQUAL(vec3(500,0,0), m_worldGrid->m_quadrants.at(0)->m_endPt.loc);
   CHECK_EQUAL(vec3(1000,0,0), m_worldGrid->m_quadrants.at(1)->m_endPt.loc);
   CHECK_EQUAL(vec3(0,0,0), m_worldGrid->m_quadrants.at(2)->m_endPt.loc);
}

TEST_FIXTURE(swgf, simpleInitGameObjs)
{
   // Check right number of units
   CHECK_EQUAL(SimpleWorldGridFixture::units, m_worldGrid->m_quadrants.at(0)->m_gameObjects.size());
   CHECK_EQUAL(0, m_worldGrid->m_quadrants.at(1)->m_gameObjects.size());
   CHECK_EQUAL(0, m_worldGrid->m_quadrants.at(2)->m_gameObjects.size());
}

TEST_FIXTURE(swgf, simpleInitObj3Ds)
{
   // Check right number of units
   CHECK_EQUAL(SimpleWorldGridFixture::obj3Ds, m_worldGrid->m_quadrants.at(0)->m_obj3Ds.size());
   CHECK_EQUAL(0, m_worldGrid->m_quadrants.at(1)->m_obj3Ds.size());
   CHECK_EQUAL(0, m_worldGrid->m_quadrants.at(2)->m_obj3Ds.size());
}

/*
 setPlayer() tests
*/


/*
 placeInGrid() tests
*/


/*
 placeInCurrQuadrant() tests
*/


/*
 tic() tests
*/


/*
 checkCollisions() tests
*/


/*
 getCurrentQuadrant() tests
*/


/*
 getDrawableObjects()
*/


/*
 updateObjects() tests
*/


/*
 determineQuadrant() tests
*/


