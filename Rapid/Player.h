#include "GameObject.h"
#include <typeinfo>

class Player : GameObject
{
public:

   int health = 100;
   int cooldown = 0;
   bool firing = false;
   CShader shade;

   Player(SVector3 pos, SVector3 vel, CMesh * mod, CShader shad1);
   virtual void update(float dt);
   void draw();
   void collideWith(GameObject collided);
};
