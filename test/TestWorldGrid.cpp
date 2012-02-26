//
//  TestWorldGrid.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/25/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <UnitTest++/UnitTest++.h>
#include "WorldGrid.h"
#include "ResourceManager.h"
#include "FakeGameEngine.h"
#include "FakeRenderingEngine.h"

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

struct SimpleWorldGridFixtureWithPlayer
{
   SimpleWorldGridFixtureWithPlayer()
   {
      m_modules = new Modules();
      m_modules->resourceManager = new ResourceManager();
      m_modules->gameEngine = new FakeGameEngine();
      m_modules->renderingEngine = new FakeRenderingEngine();
      m_worldData = m_modules->resourceManager->readWorldData("test/testCourse.wf");
      m_worldGrid = new WorldGrid(*m_worldData, m_modules);
      
      Player* p = new Player("models/spaceship.obj", "textures/test3.bmp", m_modules, vec3(0,0,0), vec3(0,0,1), vec3(1,0,0));
      
      m_worldGrid->setPlayer(p);
   }
   
   ~SimpleWorldGridFixtureWithPlayer()
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
int SimpleWorldGridFixtureWithPlayer::quadrants = 3;
int SimpleWorldGridFixtureWithPlayer::units = 1;
int SimpleWorldGridFixtureWithPlayer::obj3Ds = 1;

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
typedef SimpleWorldGridFixtureWithPlayer swgfwp;

typedef ComplexWorldGridFixture cwgf;

class SimpGameObj : public GameObject, public Object3d
{   
public:
   void doCollision(GameObject& other) { }
};

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
 determineQuadrant() tests
 */
TEST_FIXTURE(swgf, simpleDetQuadCurrent)
{   
   CHECK_EQUAL(0, m_worldGrid->determineQuadrant(vec3(1,0,0)));
}

TEST_FIXTURE(swgf, simpleDetQuadNext)
{
   CHECK_EQUAL(1, m_worldGrid->determineQuadrant(vec3(501,0,0)));
}

TEST_FIXTURE(swgf, simpleDetQuadLast)
{
   CHECK_EQUAL(2, m_worldGrid->determineQuadrant(vec3(1000,0,0)));
}

/*
 setPlayer() tests
*/
TEST_FIXTURE(swgf, simpleSetPlayer)
{
   Player* p = new Player("models/spaceship.obj", "textures/test3.bmp", m_modules, vec3(0,0,0), vec3(0,0,1), vec3(1,0,0));
   
   m_worldGrid->setPlayer(p);
   
   CHECK_EQUAL(p, m_worldGrid->m_player);
   
   Turret t = *((Turret*)m_worldGrid->m_quadrants.at(0)->m_gameObjects.front());
   CHECK_EQUAL(p, t.m_playerRef);
}

/*
 placeInGrid() tests
*/
TEST_FIXTURE(swgf, simplePlaceInGridGood)
{
   SimpGameObj obj;
   
   obj.setPosition(vec3(1, 0, 0));
   
   int quad = m_worldGrid->placeInGridDEBUG(&obj, &obj);
   
   CHECK_EQUAL(0, quad);
   CHECK(std::find(m_worldGrid->m_quadrants.at(0)->m_gameObjects.begin(),  
                   m_worldGrid->m_quadrants.at(0)->m_gameObjects.end(), &obj) 
         != m_worldGrid->m_quadrants.at(0)->m_gameObjects.end());
   
   obj.setPosition(vec3(501, 0, 0));
   
   quad = m_worldGrid->placeInGridDEBUG(&obj, &obj);
   
   CHECK_EQUAL(1, quad);
   CHECK(std::find(m_worldGrid->m_quadrants.at(1)->m_gameObjects.begin(),  
                   m_worldGrid->m_quadrants.at(1)->m_gameObjects.end(), &obj) 
         != m_worldGrid->m_quadrants.at(1)->m_gameObjects.end());
   
   obj.setPosition(vec3(1001, 0, 0));
   
   quad = m_worldGrid->placeInGridDEBUG(&obj, &obj);
   
   CHECK_EQUAL(2, quad);
   CHECK(std::find(m_worldGrid->m_quadrants.at(2)->m_gameObjects.begin(),  
                   m_worldGrid->m_quadrants.at(2)->m_gameObjects.end(), &obj) 
         != m_worldGrid->m_quadrants.at(2)->m_gameObjects.end());
}

/*
 placeInCurrQuadrant() tests
*/
TEST_FIXTURE(swgf, simpPlaceInCurrQuadGood)
{
   SimpGameObj obj;
   
   obj.setPosition(vec3(1, 0, 0));
   
   int quad = m_worldGrid->placeInCurrQuadrantDEBUG(&obj, &obj);
   
   CHECK_EQUAL(0, quad);
   CHECK(std::find(m_worldGrid->m_quadrants.at(0)->m_gameObjects.begin(),  
                   m_worldGrid->m_quadrants.at(0)->m_gameObjects.end(), &obj) 
         != m_worldGrid->m_quadrants.at(0)->m_gameObjects.end());
}


/*
 tic() tests
*/
TEST_FIXTURE(swgfwp, simpleTicCurrQuad)
{
   m_worldGrid->tic(100);
   
   CHECK_EQUAL(0, m_worldGrid->m_currentQuadrant);
    
   ((FakeGameEngine*)m_modules->gameEngine)->m_camera->setPosition(vec3(900, 0, 0));
   
   m_worldGrid->tic(100);
   
   CHECK_EQUAL(1, m_worldGrid->m_currentQuadrant);
}

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

