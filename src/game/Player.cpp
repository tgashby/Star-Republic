#include "Player.h"

#define VCHANGE 0.8
#define VINTENS 0.5
#define SCREENX 400
#define SCREENY 300

Player::Player(string fileName, string textureName, Modules *modules, 
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw) 
   :  Object3d(), Flyer(),
      health(100), side(1,0,0), 
      lastScreenX(0), lastScreenY(0)
{
   m_forward = cam_forw;
   m_up = cam_up;
   m_position = cam_up;
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

   mat4 modelMtx = mat4::Scale(MODEL_SCALE) * 
      mat4::Magic(-getForward(), getUp(), getPosition());
   m_mesh->setModelMtx(modelMtx);
}

Player::~Player()
{

}


//All Vectors are updated in here
void Player::tic(uint64_t time, Vector3<float> cam_position, Vector3<float> cam_up, Vector3<float> cam_forward)
{
  mat4 modelMtx;
  Vector3<float> tempPos;

 //m_forward = cam_forward;	
 //m_up = cam_up;
 m_forward = cam_forward.Normalized();
 side = cam_forward.Cross(cam_up);
 side.Normalize();
 m_up = side.Cross(m_forward);
 //m_position = cam_position + (cam_forward * PLAYER_DISTANCE_FROM_CAMERA);
 tempPos = cam_position + (cam_forward * PLAYER_DISTANCE_FROM_CAMERA); 
 calculateSide();
 m_sideVelocity = (((side * lastScreenX)) - (m_position - tempPos)) * X_SCALAR;
 m_upVelocity = (((m_up * lastScreenY)) - (m_position - tempPos)) * Y_SCALAR;
 m_offsetPosition += (m_sideVelocity * time) + (m_upVelocity * time);
 m_position = m_offsetPosition + tempPos;

 modelMtx = mat4::Scale(MODEL_SCALE) * 
      mat4::Magic(-(((m_sideVelocity + m_upVelocity + m_forward)/3).Normalized()), m_up, m_position);
   m_mesh->setModelMtx(modelMtx);

   x += vx * time;
   y += vy * time;
   updateVelocity(lastScreenX, lastScreenY);
   cerr << "Ship is " << m_position.x << " " << m_position.y << " " << m_position.z << "\n";
   cerr << "Cam Forward is " << cam_forward.x << " " << cam_forward.y << " " << cam_forward.z << "\n";
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
   float tvx = vx * VINTENS;
   float tvy = vy * VINTENS;
   return (-getForward() * (1 - abs(tvx)) * (1 - abs(tvy)) 
      + getSide() * tvx + getUp() * tvy).Normalized();
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
   float tvy = vy * VINTENS;
   return (getUp() * (1 - abs(tvy)) + getForward() * tvy).Normalized();
}

Vector3<float> Player::getSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
   return side;
}

void Player::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}

void Player::doCollision(GameObject & other){
   cerr << "I'm hit!\n";
   //DO Collision stuff
}
