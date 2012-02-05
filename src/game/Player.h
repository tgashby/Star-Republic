#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"
#define VELOCITY 0.002f

class Player : public Object3d {
public:
   Player(string fileName, string textureName, Modules *modules);
   ~Player();
   void tic(uint64_t time);
   void setVelocity(Vector3<float> vel);
   void setPosition(Vector3<float> pos);
   void setAcceleration(Vector3<float> acc);
   void setBearing(Vector3<float> current);
   Vector3<float> getPosition();
   Vector3<float> getFoward();
private:
   int health;
   Mesh *m_mesh;
   Vector3<float> position;
   Vector3<float> velocity;
   Vector3<float> forward;
   Vector3<float> acceleration;
};

#endif
