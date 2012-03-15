#include "Player.h"
#include "Bullet.h"
#include "Turret.h"
#include "EnemyShip.h"
#include "EnemyGunship.h"

#define VCHANGE 0.8
#define VINTENS 0.5
#define SCREENX 400
#define SCREENY 300

const int Player::health = 200;

/** Constructor **/

Player::Player(string fileName, string textureName, Modules *modules, 
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw) 
   :  Object3d(), Flyer(), m_side(1,0,0), lastScreenX(0), lastScreenY(0), m_sideVelocity(0,0,0), m_upVelocity(0,0,0)
{
  m_forward = cam_forw.Normalized();
  m_up = cam_up;
  m_position = cam_pos;// + (cam_forw.Normalized() * PLAYER_DISTANCE_FROM_CAMERA);
  calculateSide();

   m_shipMesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_shipMesh);
   
   m_exhaustMesh = new Mesh("models/spaceship_exhaust.obj", "textures/test4.bmp", modules);
   m_exhaustMesh->setShaderType(SHADER_BLOOM);
   m_meshList.push_back(m_exhaustMesh);
   m_isFlashing = false;
   m_count = 0;
   

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
   m_shipMesh->setModelMtx(modelMtx);
   m_exhaustMesh->setModelMtx(modelMtx);
   
   m_health = Player::health;

   magnet = true;
   m_modules = modules;
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

   if (m_isFlashing) {
      m_count++;
      int temp = m_count % 4;
      if (temp < 2 && temp > 0) {
         m_shipMesh->setVisible(false);
         m_exhaustMesh->setVisible(false);
      }
      else if (temp < 4) {
         m_shipMesh->setVisible(true);
         m_exhaustMesh->setVisible(true);
      }
      if (m_count >= 20) {
         m_count = 0;
         m_isFlashing = false;
      }
   }

   /** set the model matrix based on a constant scale and rotate and
* the forward, up and position (aka magic) **/
   modelMtx = mat4::Scale(MODEL_SCALE) * mat4::Rotate(180, vec3(0,1,0)) *
      mat4::Magic(getAimForward(), getAimUp(), m_position);
   m_shipMesh->setModelMtx(modelMtx);
   m_exhaustMesh->setModelMtx(modelMtx);

   x += vx * time;
   y += vy * time;
   updateVelocity(lastScreenX, lastScreenY);

   //cout << "Position : " << m_position.x << ", " << m_position.y << ", " <<
   //   m_position.z << "\n";
   //cout << "Forward : " << m_forward.x << ", " << m_forward.y << ", " <<
   //   m_forward.z << "\n";
}

void Player::updateVelocity(float diffX, float diffY)
{
   /** **/
   vx = -(diffX + x - SCREENX) / SCREENX;
   vy = -(diffY + y - SCREENY) / SCREENY;

   lastScreenX = diffX;
   lastScreenY = diffY;
}

Vector3<float> Player::getMagneticForward()
{
   if (magnet)
      return magnetic;
   else
      return getAimForward();
}

void Player::setMagneticForward(vec3 dir)
{
   magnetic = dir;
}

void Player::toggleMagnetic()
{
   magnet = !magnet;
}

float Player::getHealthPercent() {
  return (m_health * 1.0f) / (health * 1.0f);
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
   //DO Collision stuff
   if (typeid(other) == typeid(Bullet))
   {
      if (&((Bullet&)other).getParent() != this)
      {
         m_health -= 2;
         m_isFlashing = true;
         m_modules->soundManager->playSound(PlayerHit); 
      }
   }
   else if (typeid(other) == typeid(Turret))
   {
      m_health -= 10;
      m_isFlashing = true;
   }
   else if (typeid(other) == typeid(EnemyGunship)){
      m_health -= 15;
      m_isFlashing = true;
   }
   else if (typeid(other) == typeid(EnemyGunship)){
      m_health -= 15;
      m_isFlashing = true;
   }
   
   if (m_health <= 0) {
       m_alive = false;
       m_shipMesh->setVisible(false);
       m_exhaustMesh->setVisible(false);
   }
}

bool Player::getAlive() {
   return m_alive;
}

void Player::setVisible(bool visibility) {
   m_shipMesh->setVisible(visibility);
   m_exhaustMesh->setVisible(visibility);
}

vec3 Player::getOffSet() {
  return m_offsetPosition;
}

vec3 Player::getMForward()
{
  return m_forward;
}
