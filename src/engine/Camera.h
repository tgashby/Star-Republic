#ifndef StarRepub_Camera_h
#define StarRepub_Camera_h

#include "Interfaces.h"
#include "../game/WorldPoint.h"
#include "../game/Player.h"
#define CAMERA_DIST_FROM_PLAYER 150
#define CAMERA_LOOK_AHEAD_DISTANCE 200.0f
#define SHIP_CAMERA_ABOVE_SCALAR 30
#define CAMERA_DEF_VELOCITY 0.3f
#define CAMERA_BOOST_VELOCITY 0.50f

#define _MOTION_CAMERA 1
#define _PATH_CAMERA 2
#define _SHIP_CAMERA 3
#define _FPS_CAMERA 4

#include <assert.h>
#include <iostream>

class Player;

class Camera: public ICamera {
 public:
   Camera(WorldPoint* head, WorldPoint* tail);
   ~Camera();
   
   mat4 getProjectionViewMtx();
   void tic(uint64_t time);
   void checkPath(WorldPoint* head);
   void setCameraType(int cam);
   void setPlayer(Player* player);
   void setBoosting(bool boostStatus);
   
   vec3 getPosition();
   vec3 getRef();
   vec3 getForward();
   vec3 getUp();
   
 private:
   //Sets the true look at vectors based on camera type
   void setLookAt();
   //Updates the m_pathSide based on our up and forward
   void calculateSide();
   //Basic angle calculation methods
   float angleBetween(vec3 one, vec3 two);
   
   //Actual camera look at vectors.  These get set based on
   // the type of camera we want
   vec3 m_eye;
   vec3 m_ref;
   vec3 m_up;
   
   //Head and tail of our current path
   // (imagine an arrow - the head is the triangle, where it is pointing)
   WorldPoint* m_tail;
   WorldPoint* m_head;
   
   //Indicates whether we are turning our forward or not
   bool m_turning;
   bool m_boosting;
   
   //Path variables are the progress along the path
   float m_pathAngle;
   vec3 m_pathSide;
   vec3 m_pathUp;
   vec3 m_pathPos;
   vec3 m_pathRef;
   vec3 m_pathForw;
   
   //The camera we are currently using
   int cameraType;

   //Some camera's require the player's position
   Player* m_player;
   
};

#endif
