#include "Player.h"
#include "Bullet.h"
#include "Turret.h"

#define VCHANGE 0.8
#define VINTENS 0.5
#define SCREENX 400
#define SCREENY 300

/** Constructor **/

Player::Player(string fileName, string textureName, Modules *modules, 
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw) 
   :  Object3d(), Flyer(), m_side(1,0,0), lastScreenX(0), lastScreenY(0), m_sideVelocity(0,0,0), m_upVelocity(0,0,0)
{
   m_forward = cam_forw;
   m_up = cam_up;
   m_position = cam_pos;
   calculateSide();

   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   // these are relative to the 'forward' vector
   x = SCREENX;
   y = SCREENY;

   SDL_WarpMouse( x, y );
   vx = 0;
   vy = 0;

   /** set the model matrix based on a constant scale and rotate and
* the forward, up and position (aka magic) **/
   mat4 modelMtx = mat4::Scale(MODEL_SCALE) * mat4::Rotate(180, vec3(0,1,0)) *
      mat4::Magic(getForward(), getUp(), getPosition());
   m_mesh->setModelMtx(modelMtx);
   
   // god mode much?
   m_health = 200000;
}

Player::~Player()
{

}


//All Vectors are updated in here
void Player::tic(uint64_t time, Vector3<float> cam_position, Vector3<float> cam_up, Vector3<float> cam_forward)
{
   mat4 modelMtx;
   Vector3<float> tempPos;

   m_forward = cam_forward.Normalized();
   m_up = cam_up.Normalized();
   calculateSide();


   tempPos = cam_position + (cam_forward * PLAYER_DISTANCE_FROM_CAMERA); 
   
   m_sideVelocity = (((m_side * lastScreenX)) - (m_position - tempPos)) * X_SCALAR;

   m_upVelocity = (((m_up * lastScreenY)) - (m_position - tempPos)) * Y_SCALAR;
   m_offsetPosition += (m_sideVelocity * time) + (m_upVelocity * time);
   m_position = m_offsetPosition + tempPos;

   /** set the model matrix based on a constant scale and rotate and
* the forward, up and position (aka magic) **/
   modelMtx = mat4::Scale(MODEL_SCALE) * mat4::Rotate(180, vec3(0,1,0)) *
      mat4::Magic(getAimForward(), getAimUp(), m_position);
   m_mesh->setModelMtx(modelMtx);

   x += vx * time;
   y += vy * time;
   updateVelocity(lastScreenX, lastScreenY);
}

void Player::updateVelocity(float diffX, float diffY)
{
   /** **/
   vx = -(diffX + x - SCREENX) / SCREENX;
   vy = -(diffY + y - SCREENY) / SCREENY;

   lastScreenX = diffX;
   lastScreenY = diffY;
}

Vector3<float> Player::getAimForward()
{
   /** make sure the velocity in X and Y aren't to great -
* this will translate into not having the player rotate to far
* in any direction, and not move too fast in 'screen coordinates' **/
   if (vx > VCHANGE)
      vx = VCHANGE;
   if (vy > VCHANGE)
      vy = VCHANGE;
   if (vx < -VCHANGE)
      vx = -VCHANGE;
   if (vy < -VCHANGE)
      vy = -VCHANGE;
   /** Multiply the velocity in X and Y by a scalar -
* this gives us control over how fast the player can move and rotate **/
   float tvx = -vx * VINTENS;
   float tvy = -vy * VINTENS;
   /** Return a vector interpolated from the actual forward, the
* up (velocity in Y) and the side (velocity in X) **/
   return (m_forward * (1 - abs(tvx)) * (1 - abs(tvy))
      + getSide() * tvx + m_up * tvy).Normalized();
}
Vector3<float> Player::getAimUp()
{
   /** make sure the velocity in Y isn't to great -
* this will translate into not having the player rotate to far
* in any direction, and not move too fast in 'screen coordinates' **/
   if (vx > VCHANGE)
      vx = VCHANGE;
   if (vy > VCHANGE)
      vy = VCHANGE;
   if (vx < -VCHANGE)
      vx = -VCHANGE;
   if (vy < -VCHANGE)
      vy = -VCHANGE;
   /** Multiply the velocity in Y by a scalar -
* this gives us control over how fast the player can move and rotate **/
   float tvy = -vy * VINTENS;
   /** Return a vector interpolated from the actual up and the forward **/
   return (m_up * (1 - abs(tvy)) + m_forward * tvy).Normalized();
}

Vector3<float> Player::getSide()
{
   m_side = m_up.Cross(m_forward).Normalized();
   return m_side;
}

void Player::calculateSide() {
   m_side = m_up.Cross(m_forward).Normalized();
}

void Player::doCollision(GameObject & other)
{
   if (typeid(other) == typeid(Bullet))
   {
      if (&((Bullet&)other).getParent() != this)
      {
         m_health -= 2;
      }
   }
   
   if (typeid(other) == typeid(Turret))
   {
     m_health -= 10;
   }
   
   if (m_health <= 0) {
       m_alive = false;
       m_mesh->setVisible(false);
   }
   //DO Collision stuff
}

void Player::setVisible(bool visibility) {
   m_mesh->setVisible(visibility);
}

vec3 Player::getOffSet() {
  return m_offsetPosition;
}

vec3 Player::getMForward()
{
  return m_forward;
}
