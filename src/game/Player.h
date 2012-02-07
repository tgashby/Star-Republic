#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"
#include <assert.h>
#include <cmath>
#define VELOCITY 0.02f

class Player : public Object3d {
public:
   Player(string fileName, string textureName, Modules *modules);
   ~Player();
   void tic(uint64_t time);
   void setProgressVelocity(Vector3<float> vel);
   void setPosition(Vector3<float> pos);
   void setAcceleration(Vector3<float> acc);
   void setBearing(Vector3<float> headPos, Vector3<float> headUp);
   void setProgress(Vector3<float> pos);
   void setUp(Vector3<float> upVal);
   void calculateSide();
   Vector3<float> getPosition();
   Vector3<float> getForward();
   Vector3<float> getUp();
   Vector3<float> getSide();
   Vector3<float> getProgress();
   Vector3<float> getProgressVelocity();
   Vector3<float> updateVelocity(float diffX, float diffY);
   void setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp, Vector3<float> prevHeadPos,
   Vector3<float> prevHeadUp);
   
private:
   int health;
   Mesh *m_mesh;
   Vector3<float> position;
   Vector3<float> progressVelocity;
   Vector3<float> shipVelocity;
   Vector3<float> forward;
   Vector3<float> acceleration;
   Vector3<float> progress;
   Vector3<float> up;
   Vector3<float> side;
	Vector3<float> currentHeadPos;
	Vector3<float> previousHeadPos;
	Vector3<float> currentHeadUp;
	Vector3<float> previousHeadUp;
   float lastScreenX;
   float lastScreenY;
	float prevAngle;
	float currentAngle;
	
};

#endif
