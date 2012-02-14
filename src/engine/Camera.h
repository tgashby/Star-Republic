#ifndef StarRepub_Camera_h
#define StarRepub_Camera_h

#include "Interfaces.h"
#include "../game/WorldPoint.h"
#define CAMERA_DIST_FROM_PLAYER 160
#define CAMERA_LOOK_AHEAD_DISTANCE 200.0f
#define CAMERA_REF_VELOCITY 0.3f

#define _PATH_CAMERA 1
#define _MOTION_CAMERA 2
#define _SHIP_CAMERA 3
#define _FPS_CAMERA 4

#include <assert.h>
#include <iostream>

/**
 * The game's camera.  This class not only controls our view, 
 * but our movement along the path.
 */

class Camera: public ICamera {
 public:

  /**
   * Camera constructor.  Builds the camera based on the starting point and the
   * head.
   * @param head - The head of the path, when thinking of it as an arrow.
   * @param tail - The tail end of the path.  This is the PathPoint that we start
   * at.
   */
  Camera(WorldPoint* head, WorldPoint* tail);

  /**
   * Standard deconstructor for the camera.
   */
  ~Camera();
  
  /**
   * Builds the projection view matrix for the RenderingEngine based on the given vectors.
   * @return - The matrix required for our currentl
   */
  mat4 getProjectionViewMtx();

  /**
   * Increments the camera's according the the time that has passed
   */
  void tic(uint64_t time);
  
  /**
   * Checks that we are moving towards the correct path.
   * If not, it changes the forward vector.
   * @param head - The head of the current path we should be following
   */
  void checkPath(WorldPoint* head);

  /**
   * Calculates the angle between two vectors.
   * Probably will be moved elsewhere later, since it is a useful function.
   */
  float angleBetween(vec3 one, vec3 two);

  /**
   * Gives the current position along the path that the camera is.
   * @return The position of the camera along the path.
   */
  vec3 getPosition();

  /**
   * Gives the current reference point we are looking along the path.
   * @return The reference point along the path we are looking at.
   */
  vec3 getRef();
  
  /**
   * Gives the vecor from the camera's movement reference to the camera's path position.
   * @return The forward vector of the camera's movement.
   */
  vec3 getForward();

  /** 
   * Gives the current up vector along the path.
   * @return the up vector of the camera's position along the path.
   */
  vec3 getUp();

 private:

  /** 
   * Calculates the camera's side vector; this is used when the up and forward
   * are rotated.
   */
  void calculateSide();

  /**
   * Calculates the camera's actual look at vectors based on what type of camera it is.
   */
  void setLookAt();

  /** 
   * Actual camera position (i.e. where our view is centered from
   */
  vec3 m_eye;

  /**
   * The actual position our view is looking at.
   */
  vec3 m_ref;

  /**
   * The actual up vector of our view.
   */
  vec3 m_up;

  /**
   * The tail of our path (i.e. the most recent point we passed)
   */
  WorldPoint* m_tail;
  /**
   * The head of our path (i.e. the point we are currently heading toward
   */
  WorldPoint* m_head;

  /**
   * Indicates whether we are turning or not.
   */
  bool m_turning;
  
  /**
   * The angle our path requires us to rotate around.
   */
  float m_pathAngle;
  
  /** 
   * The side vecctor of our camera
   */
  vec3 m_pathSide;

  /**
   * The up vector of our camera
   */
  vec3 m_pathUp;

  /**
   * The position along the path our camera is
   */
  vec3 m_pathPos;

  /**
   * The reference point our path camera is currently looking at.
   * Used especially for smooth turningin.
   */
  vec3 m_pathRef;

  /**
   * The forward vector of our camera's path
   */
  vec3 m_pathForw;

  /**
   * The type of camera we are currently using.
   * It doesn't actually change our path position.
   */
  int cameraType;

};

#endif
