#ifndef StarRepub_Camera_h
#define StarRepub_Camera_h

#include "Interfaces.h"
#include "../game/WorldPoint.h"
#define CAMERA_DIST_FROM_PLAYER 160
#define CAMERA_LOOK_AHEAD_DISTANCE 200.0f
#define CAMERA_REF_VELOCITY 0.3f

#include <assert.h>
#include <iostream>

class Camera: public ICamera {
 public:
  Camera(WorldPoint* head, WorldPoint* tail);
  ~Camera();
  mat4 getProjectionViewMtx();
  void rotLocal(float pitch, float yaw);
  void moveInOut(float dist);
  void calculateSide();
  void tic(uint64_t time);
  void checkPath(WorldPoint* head);
  float angleBetween(vec3 one, vec3 two);
  vec3 getPosition();
  vec3 getRef();
  vec3 getForward();
  vec3 getUp();

 private:
  vec3 m_eye;
  vec3 m_ref;
  vec3 m_up;
  vec3 m_forw;
  WorldPoint* m_tail;
  WorldPoint* m_head;
  bool m_turning;
  float m_pathAngle;
  vec3 m_side;
  //vec3 m_forward;
};

#endif
