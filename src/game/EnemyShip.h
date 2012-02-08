#ifndef StarRepub_EnemyShip_h
#define StarRepub_EnemyShip_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include "Flyer.h"
#include "Mesh.h"
#include <assert.h>
#include <cmath>

const float vELOCITY = 0.02f;
const float x_SCALAR = 0.0005f; 
const float y_SCALAR = 0.0005f; 
const float mODEL_SCALE = 0.8f;

class EnemyShip : public Object3d, public Flyer {
public:
   EnemyShip(string fileName, string textureName, Modules *modules);
   ~EnemyShip();
   void tic(uint64_t time); //GOBJ/Flyer
//   void setProgressVelocity(Vector3<float> vel); Flyer
//   void setPosition(Vector3<float> pos); GOBJ
//   void setAcceleration(Vector3<float> acc); GOBJ
   void setBearing(Vector3<float> headPos, Vector3<float> headUp); //Flyer
//   void setProgress(Vector3<float> pos); Flyer
//   void setUp(Vector3<float> upVal); GOBJ
   Vector3<float> getSide();
   void calculateSide();
//   Vector3<float> getPosition(); GOBJ
//   Vector3<float> getForward(); GOBJ/Flyer
//   Vector3<float> getUp(); GOBJ
//   Vector3<float> getProgress(); Flyer
//   Vector3<float> getProgressVelocity(); Flyer
   void updateVelocity(float diffX, float diffY); //GOBJ
   Vector3<float> getAimForward();
   Vector3<float> getAimUp();
//   void setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp,
//                 Vector3<float> prevHeadPos, Vector3<float> prevHeadUp); Flyer
   
private:
   int health;
   // for aim and rotation, consider the player on a plane normal to the forward
   float vx, vy; 
   float x, y;
   int prevX, prevY; // for keeping track of the mouse, also for aim
   Mesh *m_mesh;
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
   float lastScreenX;
   float lastScreenY;
   float prevAngle;
   float currentAngle;
	
};

#endif
