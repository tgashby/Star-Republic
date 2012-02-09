#include "EnemyShip.h"

#define VCHANGE 0.8
#define VINTENS 0.5
#define SCREENX 400
#define SCREENY 300
#define MAXSCAREDANGLE 15
#define MAXSCAREDSPEED 0.3

EnemyShip::EnemyShip(string fileName, string textureName, Modules *modules, Player &p) 
   :  Object3d(), Flyer(), Enemy(p),
      health(100), side(1,0,0), 
      currentAngle(0), prevAngle(0)
{
  m_mesh = new Mesh(fileName, textureName, modules);
  m_meshList.push_back(m_mesh);

  dpos = (m_playerRef.getPosition() - m_position).Normalized();

  dodging = false;
  dodgedir = 0;

  mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
     mat4::Magic(-getForward(), getUp(), getPosition());
  m_mesh->setModelMtx(modelMtx);
}

EnemyShip::~EnemyShip()
{
   delete m_mesh;
}


//All Vectors are updated in here
void EnemyShip::tic(uint64_t time)
{
  dpos = (m_playerRef.getPosition() - m_position).Normalized();
  
  // moving based on the player's direction and it's aiming direction
  m_position += m_playerRef.getForward() * PATHVELOCITY + getAimForward() * AIMVELOCITY; 

  // 'scared ship' AI
  float aimAngle;
  aimAngle = 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef.getAimForward()));
  if (aimAngle < MAXSCAREDANGLE)
  {
     if (!dodging)
     {
        if (180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef.getSide())) < 90)
           dodgedir = 1;
        else
           dodgedir = -1;
     }
     vec3 motionDir = getScaredSide() / (aimAngle / MAXSCAREDANGLE) * MAXSCAREDSPEED * dodgedir;
     m_position += motionDir;
     dodging = true;
  }
  else
  {
     dodging = false;
     dodgedir = 0;
  }

  mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0));
  modelMtx *= mat4::Magic(getAimForward(), getAimUp(), getPosition());
  m_mesh->setModelMtx(modelMtx);
}

void EnemyShip::setBearing(Vector3<float> headPosition, Vector3<float> headUp)
{
   if (headPosition.x != m_currentHeadPos.x || headPosition.y != m_currentHeadPos.y 
       || headPosition.z != m_currentHeadPos.z) {
      m_previousHeadPos = m_currentHeadPos;
      m_currentHeadPos = headPosition;
      m_previousHeadUp = m_currentHeadUp;
   }
   
   m_currentHeadUp = headUp.Normalized();
   
   m_progressVelocity = (headPosition - m_progress).Normalized() * PATHVELOCITY;
   
   prevAngle = currentAngle;
   currentAngle = 180.0f / 3.14159265f * acos(m_previousHeadUp.Normalized().Dot(headUp.Normalized()));
}

Vector3<float> EnemyShip::getAimForward()
{
   return dpos;
}
Vector3<float> EnemyShip::getAimUp()
{
   return getUp();
}

Vector3<float> EnemyShip::getScaredSide()
{
   vec3 aaargh = m_playerRef.getAimForward().Cross(m_playerRef.getAimUp()).Normalized();
   return aaargh;
}


Vector3<float> EnemyShip::getSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
   return side;
}

void EnemyShip::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}
