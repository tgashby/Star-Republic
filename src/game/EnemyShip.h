#ifndef StarRepub_EnemyShip_h
#define StarRepub_EnemyShip_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include "Flyer.h"
#include "Mesh.h"
#include "Enemy.h"
#include <assert.h>
#include <cmath>

const float vELOCITY = 0.02f;
const float x_SCALAR = 0.0005f; 
const float y_SCALAR = 0.0005f; 
const float mODEL_SCALE = 0.05f;
const float ROTATE_CONSTANT = 90;

class EnemyShip : public Object3d, public Flyer, public Enemy {
public:
   EnemyShip(string fileName, string textureName, Modules *modules, Player &p);
   ~EnemyShip();
   void tic(uint64_t time); //GOBJ/Flyer
   void setBearing(Vector3<float> headPos, Vector3<float> headUp); //Flyer
   Vector3<float> getSide();
   void calculateSide();
   void updateVelocity(float diffX, float diffY); //GOBJ
   Vector3<float> getAimForward();
   Vector3<float> getAimUp();
   
private:
   int health;
   Mesh *m_mesh;
   Vector3<float> side;
	
};

#endif
