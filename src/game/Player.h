#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"
#define VELOCITY 0.02f

class Player : public Object3d {
public:
   Player(string fileName, string textureName, Modules *modules);
   ~Player();
   void tic(uint64_t time);
   void setProgressVelocity(Vector3<float> vel);
   void setPosition(Vector3<float> pos);
   void setAcceleration(Vector3<float> acc);
   void setBearing(Vector3<float> current);
   void setProgress(Vector3<float> pos);
   void setUp(Vector3<float> upVal);
   void calculateSide();
   Vector3<float> getPosition();
   Vector3<float> getForward();
   Vector3<float> getUp();
   Vector3<float> getProgress();
   Vector3<float> updateVelocity(float diffX, float diffY);
   
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
   float lastScreenX;
   float lastScreenY;
};

#endif
