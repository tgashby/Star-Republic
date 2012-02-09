#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include "Flyer.h"
#include "Mesh.h"
#include <assert.h>
#include <cmath>
#define VELOCITY 0.04f
#define X_SCALAR 0.0005f
#define Y_SCALAR 0.0005f
#define MODEL_SCALE 0.8f
#define PLAYER_DISTANCE_FROM_CAMERA 120

class Player : public Object3d, public Flyer {
public:
   Player(string fileName, string textureName, Modules *modules, 
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw);
   ~Player();
   void tic(uint64_t time, Vector3<float> cam_position, Vector3<float> cam_up, Vector3<float> cam_forward); //GOBJ/Flyer
//   void setProgressVelocity(Vector3<float> vel); Flyer
//   void setPosition(Vector3<float> pos); GOBJ
//   void setAcceleration(Vector3<float> acc); GOBJ
//   void setBearing(Vector3<float> headPos, Vector3<float> headUp); //Flyer
//   void setProgress(Vector3<float> pos); Flyer
//   void setUp(Vector3<float> upVal); GOBJ
//   void setFutureProgress(Vector3<float> pos);
   Vector3<float> getSide();
   void calculateSide();
//   Vector3<float> getPosition(); GOBJ
//   Vector3<float> getForward(); GOBJ/Flyer
//   Vector3<float> getUp(); GOBJ
//   Vector3<float> getProgress(); Flyer
//   Vector3<float> getProgressVelocity(); Flyer
   void doCollision(GameObject & other);
   void updateVelocity(float diffX, float diffY); //GOBJ
   Vector3<float> getAimForward();
   Vector3<float> getAimUp();
//   void setFuturePosition(Vector3<float> pos);
//   void setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp,
//                 Vector3<float> prevHeadPos, Vector3<float> prevHeadUp); Flyer
   
private:
   // for aim and rotation, consider the player on a plane normal to the forward
   float vx, vy; 
   float x, y;
   Vector3<float> xPosition, yPosition;
   Mesh *m_mesh;
   Vector3<float> m_shipVelocity;
   Vector3<float> m_sideVelocity;
   Vector3<float> m_upVelocity;
   Vector3<float> m_offsetPosition;
   //Vector3<float> position; GOBJ
   //Vector3<float> progressVelocity; Flyer
   //Vector3<float> shipVelocity; GOBJ
   //Vector3<float> forward; GOBJ
   //Vector3<float> acceleration; GOBJ
   //Vector3<float> progress; Flyer
   //Vector3<float> up; GOBJ
   Vector3<float> side;
//	Vector3<float> currentHeadPos; Flyer
//	Vector3<float> previousHeadPos; Flyer
//	Vector3<float> currentHeadUp; Flyer
//	Vector3<float> previousHeadUp; Flyer
   Vector3<float> futureProgress;
   float lastScreenX;
   float lastScreenY;
   float prevAngle;
   float currentAngle;
};

#endif
