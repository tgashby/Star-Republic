#include "Player.h"

Player::Player(string fileName, string textureName, Modules *modules) 
   :  Object3d(), Flyer(),
      health(100), side(1,0,0), 
      lastScreenX(0), lastScreenY(0), 
      currentAngle(0), prevAngle(0)
{
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   // these are relative to the 'forward' vector
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
void Player::tic(uint64_t time)
{
  mat4 tempMatrix;
  float diffAngle, upRotateAngle, forwardRotateAngle, testAngle;
  vec3 tempUp, acosTest;
  mat4 modelMtx;

  m_progressVelocity += m_acceleration * time;
   currentAngle = currentAngle * (m_previousHeadPos - m_progress).Length() / 
     (m_currentHeadPos - m_previousHeadPos).Length();
   diffAngle = currentAngle - prevAngle;

   //Gets the matrix that you need to use to make the position rotate around at this angle
   tempMatrix = mat4::Rotate(diffAngle, (m_currentHeadPos - m_progress).Normalized());	

   //Matrix multiplication to come up with the rotated up.
   tempUp.x = (m_up.x * tempMatrix.x.x) + (m_up.y * tempMatrix.y.x) 
     + (m_up.z * tempMatrix.z.x) + (1 * tempMatrix.w.x);
   tempUp.y = (m_up.x * tempMatrix.x.y) + (m_up.y * tempMatrix.y.y) 
     + (m_up.z * tempMatrix.z.y) + (1 * tempMatrix.w.y);
   tempUp.z = (m_up.x * tempMatrix.x.z) + (m_up.y * tempMatrix.y.z) 
     + (m_up.z * tempMatrix.z.z) + (1 * tempMatrix.w.z);

   //Up value set to the vector that was calculated above.
   m_up = tempUp.Normalized();
   calculateSide();
   
   //TempUp is actually Tempposition here.
   tempUp.x = (m_position.x * tempMatrix.x.x) + (m_position.y * tempMatrix.y.x) 
     + (m_position.z * tempMatrix.z.x) + (1 * tempMatrix.w.x);
   tempUp.y = (m_position.x * tempMatrix.x.y) + (m_position.y * tempMatrix.y.y) 
     + (m_position.z * tempMatrix.z.y) + (1 * tempMatrix.w.y);
   tempUp.z = (m_position.x * tempMatrix.x.z) + (m_position.y * tempMatrix.y.z) 
     + (m_position.z * tempMatrix.z.z) + (1 * tempMatrix.w.z);
   
   m_position = tempUp;
   m_velocity = (((side * lastScreenX)) - (m_position - m_progress)) * X_SCALAR
      + (((m_up * lastScreenY)) - (m_position - m_progress)) * Y_SCALAR 
      + m_progressVelocity;
   m_progress += m_progressVelocity * time;
   m_position = m_position + (m_velocity * time);

   modelMtx = mat4::Scale(MODEL_SCALE) * 
      mat4::Magic(getAimForward(), getAimUp(), getPosition());
   m_mesh->setModelMtx(modelMtx);

}

void Player::setBearing(Vector3<float> headPosition, Vector3<float> headUp)
{
  if (headPosition.x != m_currentHeadPos.x || headPosition.y != m_currentHeadPos.y 
      || headPosition.z != m_currentHeadPos.z) {
    m_previousHeadPos = m_currentHeadPos;
    m_currentHeadPos = headPosition;
    m_previousHeadUp = m_currentHeadUp;
  }
  
  m_currentHeadUp = headUp.Normalized();
  
  m_progressVelocity = (headPosition - m_progress).Normalized() * VELOCITY;
  
  prevAngle = currentAngle;
  currentAngle = 180.0f / 3.14159265f * acos(m_previousHeadUp.Normalized().Dot(headUp.Normalized()));
}

void Player::updateVelocity(float diffX, float diffY)
{
   vx = -diffX / 400;
   vy = -diffY / 400;

   lastScreenX = diffX;
   lastScreenY = diffY;
}

Vector3<float> Player::getAimForward()
{
   return (-getForward() * (1 - vx) * (1 - vy) 
      + getSide() * vx + getUp() * vy).Normalized();
}
Vector3<float> Player::getAimUp()
{
   return (getUp() * (1 - vy) + getForward() * vy).Normalized();
}

Vector3<float> Player::getSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
   return side;
}

void Player::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}
