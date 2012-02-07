#include "Player.h"

Player::Player(string fileName, string textureName, Modules *modules) 
   :  Object3d(), Flyer(),
      health(100), side(1,0,0), 
      lastScreenX(0), lastScreenY(0), 
      currentAngle(0), prevAngle(0)
{
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx = mat4::Translate(0, 0, -100);
   modelMtx = mat4::Rotate(vec3(0, 0, 1), vec3(0, 1, 0)) * modelMtx;

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

   m_progressVelocity.x += m_acceleration.x * time;
   m_progressVelocity.y += m_acceleration.y * time;
   m_progressVelocity.z += m_acceleration.z * time;

   cout << "Angle is first: " << currentAngle << "\n";
   currentAngle = currentAngle * (m_previousHeadPos - m_progress).Length() / (m_currentHeadPos - m_previousHeadPos).Length();


   cout << "Angle is then: " << currentAngle << "\n";
   assert(currentAngle == currentAngle);
   diffAngle = currentAngle - prevAngle;
   cout << "Ship velocity is X: " << m_velocity.x << " Y: " << m_velocity.y << " Z: " << m_velocity.z << "\n";

     //Gets the matrix that you need to use to make the position rotate around at this angle
     // LOOK AT THIS
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

   m_velocity = (((side * lastScreenX)) - (m_position - m_progress)) * 0.0005 + (((m_up * lastScreenY)) - (m_position - m_progress)) * 0.0005 + m_progressVelocity;
   assert(m_up.x == m_up.x);
   assert(side.x == side.x);

   cout << "Ship velocity : " << m_velocity.x << ", " << m_velocity.y << ", " << m_velocity.z << "\n";
   assert(m_velocity.x == m_velocity.x);
   assert(m_velocity.y == m_velocity.y);
   assert(m_velocity.z == m_velocity.z);
   
   m_progress.x += m_progressVelocity.x * time;
   m_progress.y += m_progressVelocity.y * time;
   m_progress.z += m_progressVelocity.z * time;

   m_position = m_position + (m_velocity * time);

   upRotateAngle = 180.0f / 3.14159265f * acos(vec3(0,1,0).Dot(m_up));
   forwardRotateAngle = 180.0f / 3.14159265f * acos(vec3(0,0,-1).Dot(getForward()));
   testAngle = 180.0f / 3.14159265f * acos(m_up.Dot(getForward()));

   cerr << "ANGLE FOR COMPARISON IS: " << testAngle << "\n";
   cerr << "upRotateAngle is: " << upRotateAngle << "\n";
   cerr << "forwardRotateAngle is: " << forwardRotateAngle << "\n";
   cerr << "arccos forward IS: " << acos(vec3(0,0,-1).Dot(getForward())) << "\n";

   //if (forwardRotateAngle > 88 && forwardRotateAngle < 92) {
      acosTest.x = getForward().z;
      acosTest.y = getForward().y;
      acosTest.z = getForward().x;
      cerr << "THE TEST GIVES: " << acos(vec3(0,0,-1).Dot(acosTest)) << "\n";
      if (acos(vec3(0,0,-1).Dot(acosTest)) < acos(vec3(0,0,-1).Dot(getForward()))) {
         forwardRotateAngle = 360.0f - forwardRotateAngle;
      }
    //}
   /*if ((getForward().y < 0)) {
      forwardRotateAngle = 180.0f + forwardRotateAngle;
   }*/
   //MIGHT NEED TO BE CHANGED TO ACCOUNT FOR FORWARD AS OPPOSED TO CURRENT HEADING
   cerr << "Position is: " << m_position.x << ", " << m_position.y << ", " << m_position.z << "\n";
  // if ((getForward().x >= 0 && getForward().y >= 0 && getForward().z >= 0) ||
  //    (getForward().x 
   cerr << "Forward! X is: " << getForward().x << ", Y is: " << getForward().y << ", Z is: " << getForward().z << "\n";
   //modelMtx = mat4::Rotate(m_progress - currentHeadPos, up);
   modelMtx = mat4::Rotate(upRotateAngle, vec3(0,0,-1));
   modelMtx = modelMtx * mat4::Rotate(forwardRotateAngle, vec3(0,1,0));
   modelMtx = modelMtx * mat4::Translate(m_position.x, m_position.y, m_position.z);
   m_mesh->setModelMtx(modelMtx);
   cerr << "The UP is X: " << m_up.x << ", Y: " << m_up.y << ", Z: " << m_up.z << "\n";

}

void Player::setBearing(Vector3<float> headPosition, Vector3<float> headUp)
{
	prevAngle = currentAngle;
   if (headPosition.x != m_currentHeadPos.x || headPosition.y != m_currentHeadPos.y 
    || headPosition.z != m_currentHeadPos.z) {
      m_previousHeadPos = m_currentHeadPos;
      m_currentHeadPos = headPosition;
      m_previousHeadUp = m_currentHeadUp;
   }
	m_currentHeadUp = headUp.Normalized();

   m_progressVelocity.x = headPosition.x - m_progress.x;
   m_progressVelocity.y = headPosition.y - m_progress.y;
   m_progressVelocity.z = headPosition.z - m_progress.z;

   m_progressVelocity = m_progressVelocity.Normalized();

   m_progressVelocity = m_progressVelocity * VELOCITY;

	//cout << "Up Vector : " << up.x << " " << up.y << " " << up.z << "\n";

	//cout << m_previousHeadUp.x << " " << m_previousHeadUp.y << " " << m_previousHeadUp.z << "\n";
	//assert(false);
	currentAngle = 180.0f / 3.14159265f * acos(m_previousHeadUp.Normalized().Dot(headUp.Normalized()));
	//cout << "Angle is : " << angle << "\n";
}

void Player::updateVelocity(float diffX, float diffY)
{
   //ASSUMES X AND Y PASSED IN ARE DIFFERENCES IN SDL SCREEN COORDS
   lastScreenX = diffX;
   lastScreenY = diffY;
}

void Player::calculateSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}


