#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../game/Flyer.h"
#include "../engine/Mesh.h"
#include <assert.h>
#include <cmath>
#define VELOCITY 0.04f
#define X_SCALAR 0.0005f
#define Y_SCALAR 0.0005f
#define MODEL_SCALE 0.8f
#define PLAYER_DISTANCE_FROM_CAMERA 170

class Player : public Object3d, public Flyer {
public:
   
   Player(string fileName, string textureName, Modules *modules, 
          
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw);
   ~Player();
   
   void tic(uint64_t time, Vector3<float> cam_position, Vector3<float> cam_up, Vector3<float> cam_forward);
   
   Vector3<float> getSide();
   
   void calculateSide();
   
   void setVisible(bool visibility);
   
   void doCollision(GameObject & other);
   
   void updateVelocity(float diffX, float diffY);
   
   Vector3<float> getAimForward();
   
   Vector3<float> getAimUp();

private:
   float vx, vy; 
   float x, y;
   Vector3<float> xPosition, yPosition;
   Mesh *m_mesh;
   Vector3<float> m_shipVelocity;
   Vector3<float> m_sideVelocity;
   Vector3<float> m_upVelocity;
   Vector3<float> m_offsetPosition;
   Vector3<float> side;
   Vector3<float> futureProgress;
   float lastScreenX;
   float lastScreenY;
   float prevAngle;
   float currentAngle;
};

#endif
