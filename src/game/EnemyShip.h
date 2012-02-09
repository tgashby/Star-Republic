#ifndef StarRepub_EnemyShip_h
#define StarRepub_EnemyShip_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include "Flyer.h"
#include "Enemy.h"
#include "Mesh.h"
#include "Enemy.h"
#include <assert.h>
#include <cmath>

const float PATHVELOCITY = 0.8f;
const float AIMVELOCITY = 0.2f;
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
   Vector3<float> getScaredSide();
   void calculateSide();
   void updateVelocity(float diffX, float diffY); //GOBJ
   Vector3<float> getAimForward();
   Vector3<float> getAimUp();
   void doCollision(GameObject & other); 
   
private:
   int health;
   Mesh *m_mesh;
   Vector3<float> side;
   Vector3<float> dpos;
   float prevAngle;
   float currentAngle;
   bool dodging;
   int dodgedir;
	
};

#endif
