#include "Player.h"
#include "Bullet.h"
#include "Turret.h"

#define VCHANGE 0.8
#define VINTENS 0.5
#define SCREENX 400
#define SCREENY 300

Player::Player(string fileName, string textureName, Modules *modules, 
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw) 
   :  Object3d(), Flyer(), side(1,0,0), 
      lastScreenX(0), lastScreenY(0)
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

   //SDL_ShowCursor( SDL_DISABLE );
   SDL_WarpMouse( x, y ); 
   vx = 0;
   vy = 0;

   mat4 modelMtx = mat4::Scale(MODEL_SCALE) * mat4::Rotate(180, vec3(0,1,0)) *
      mat4::Magic(getForward(), getUp(), getPosition());
   m_mesh->setModelMtx(modelMtx);
   
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
 side = cam_forward.Cross(cam_up);
 side.Normalize();
 m_up = side.Cross(m_forward);

 tempPos = cam_position + (cam_forward * PLAYER_DISTANCE_FROM_CAMERA); 
 calculateSide();
 m_sideVelocity = (((side * lastScreenX)) - (m_position - tempPos)) * X_SCALAR;
 m_upVelocity = (((m_up * lastScreenY)) - (m_position - tempPos)) * Y_SCALAR;
 m_offsetPosition += (m_sideVelocity * time) + (m_upVelocity * time);
 m_position = m_offsetPosition + tempPos;

 modelMtx = mat4::Scale(MODEL_SCALE) * mat4::Rotate(180, vec3(0,1,0)) *
      mat4::Magic(getAimForward(), getAimUp(), m_position);
   m_mesh->setModelMtx(modelMtx);

   x += vx * time;
   y += vy * time;
   updateVelocity(lastScreenX, lastScreenY);
}


void Player::updateVelocity(float diffX, float diffY)
{
   vx = -(diffX + x - SCREENX) / SCREENX;
   vy = -(diffY + y - SCREENY) / SCREENY;

   lastScreenX = diffX;
   lastScreenY = diffY;
}

Vector3<float> Player::getAimForward()
{
   if (vx > VCHANGE)
      vx = VCHANGE;
   if (vy > VCHANGE)
      vy = VCHANGE;
   if (vx < -VCHANGE)
      vx = -VCHANGE;
   if (vy < -VCHANGE)
      vy = -VCHANGE;
   float tvx = -vx * VINTENS;
   float tvy = -vy * VINTENS;
   return (m_forward * (1 - abs(tvx)) * (1 - abs(tvy)) 
      + getSide() * tvx + m_up * tvy).Normalized();
}
Vector3<float> Player::getAimUp()
{
   if (vx > VCHANGE)
      vx = VCHANGE;
   if (vy > VCHANGE)
      vy = VCHANGE;
   if (vx < -VCHANGE)
      vx = -VCHANGE;
   if (vy < -VCHANGE)
      vy = -VCHANGE;
   float tvy = -vy * VINTENS;
   return (m_up * (1 - abs(tvy)) + m_forward * tvy).Normalized();
}

Vector3<float> Player::getSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
   return side;
}

void Player::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
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
