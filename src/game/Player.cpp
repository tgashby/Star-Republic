#include "Player.h"

Player::Player(string fileName, string textureName, Modules *modules) 
   : Object3d(), progressVelocity(0,0,0), progress(0,0,0), acceleration(0,0,0), health(100), 
     position(0,0,0), up(0,1,0), side(1,0,0), shipVelocity(0,0,0), lastScreenX(0),
     lastScreenY(0), currentAngle(0), prevAngle(0)
{
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx = mat4::Translate(0, 0, -100);
   modelMtx = mat4::Rotate(vec3(0, 0, 1), vec3(0, 1, 0)) * modelMtx;
   //modelMtx = mat4::Rotate(180, vec3(0, 1, 0)) * modelMtx;

   m_mesh->setModelMtx(modelMtx);
}

Player::~Player()
{

}

//All Vectors are updated in here
void Player::tic(uint64_t time)
{
  mat4 tempMatrix;
  float diffAngle;
  vec3 tempUp;

   progressVelocity.x += acceleration.x * time;
   progressVelocity.y += acceleration.y * time;
   progressVelocity.z += acceleration.z * time;

   currentAngle = currentAngle * (previousHeadPos - progress).Length() / (currentHeadPos - previousHeadPos).Length();
   //cout << "Angle is : " << currentAngle << "\n";
   //assert(false);
   diffAngle = currentAngle - prevAngle;
   
   shipVelocity = (((side * lastScreenX)) - (position - progress)) * 0.0005 + (((up * lastScreenY)) - (position - progress)) * 0.0005 + progressVelocity;

   //cout << "Ship velocity : " << shipVelocity.Normalized().x << ", " << shipVelocity.Normalized().y << ", " << shipVelocity.Normalized().z << "\n";
   if (shipVelocity.x != 0 || shipVelocity.y != 0 || shipVelocity.z != 0) {
     tempMatrix = mat4::Rotate(diffAngle, (currentHeadPos - progress).Normalized());	

     tempUp.x = (up.x * tempMatrix.x.x) + (up.y * tempMatrix.y.x) 
       + (up.z * tempMatrix.z.x) + (1 * tempMatrix.w.x);
     tempUp.y = (up.x * tempMatrix.x.y) + (up.y * tempMatrix.y.y) 
       + (up.z * tempMatrix.z.y) + (1 * tempMatrix.w.y);
     tempUp.z = (up.x * tempMatrix.x.z) + (up.y * tempMatrix.y.z) 
       + (up.z * tempMatrix.z.z) + (1 * tempMatrix.w.z);

     up = tempUp.Normalized();
     calculateSide();

     tempUp.x = (position.x * tempMatrix.x.x) + (position.y * tempMatrix.y.x) 
       + (position.z * tempMatrix.z.x) + (1 * tempMatrix.w.x);
     tempUp.y = (position.x * tempMatrix.x.y) + (position.y * tempMatrix.y.y) 
       + (position.z * tempMatrix.z.y) + (1 * tempMatrix.w.y);
     tempUp.z = (position.x * tempMatrix.x.z) + (position.y * tempMatrix.y.z) 
       + (position.z * tempMatrix.z.z) + (1 * tempMatrix.w.z);

     position = tempUp;
   } 

     //	cout << "Up Vector : " << up.x << " " << up.y << " " << up.z << "\n";

   progress.x += progressVelocity.x * time;
   progress.y += progressVelocity.y * time;
   progress.z += progressVelocity.z * time;

   //shipVelocity = (up * lastScreenY) + (side * lastScreenX) + progressVelocity;
   position = position + (shipVelocity * time);
	//cout << "position is: " << position.x << " " << position.y << " " << position.z << "\n";
   
   //MAYBE NOT THE BEST WAY TO DO IT
   /*mat4 modelMtx = m_mesh->getModelMtx();
   modelMtx = modelMtx * mat4::Translate(shipVelocity.x * time, shipVelocity.y * time,
					 shipVelocity.z * time);
   if (shipVelocity.x != 0 || shipVelocity.y != 0 || shipVelocity.z != 0) {
     modelMtx = modelMtx * mat4::Rotate(diffAngle, (currentHeadPos - progress).Normalized()); //* modelMtx;
     }*/

   //MIGHT NEED TO BE CHANGED TO ACCOUNT FOR FORWARD AS OPPOSED TO CURRENT HEADING
   mat4 modelMtx = mat4::Rotate(currentHeadPos - progress, up);
   modelMtx = modelMtx * mat4::Translate(position.x, position.y, position.z);
   m_mesh->setModelMtx(modelMtx);
}

void Player::setProgress(Vector3<float> pos)
{
   progress.x = pos.x;
   progress.y = pos.y;
   progress.z = pos.z;
   //cerr << "WE BE AT " << progress.x << " " << progress.y << " " << progress.z << " CAP'N!\n";
}

void Player::setPosition(Vector3<float> pos)
{
   position = pos;
   m_mesh->setModelMtx(mat4::Translate(pos.x, pos.y, pos.z));
}

void Player::setProgressVelocity(Vector3<float> vel)
{
   progressVelocity.x = vel.x;
   progressVelocity.y = vel.y;
   progressVelocity.z = vel.z;
}

void Player::setAcceleration(Vector3<float> acc)
{
   acceleration.x = acc.x;
   acceleration.y = acc.y;
   acceleration.z = acc.z;
}

void Player::setBearing(Vector3<float> headPosition, Vector3<float> headUp)
{
	prevAngle = currentAngle;
   if (headPosition.x != currentHeadPos.x || headPosition.y != currentHeadPos.y 
    || headPosition.z != currentHeadPos.z) {
      previousHeadPos = currentHeadPos;
      currentHeadPos = headPosition;
      previousHeadUp = currentHeadUp;
   }
	currentHeadUp = headUp.Normalized();

   progressVelocity.x = headPosition.x - progress.x;
   progressVelocity.y = headPosition.y - progress.y;
   progressVelocity.z = headPosition.z - progress.z;

   progressVelocity = progressVelocity.Normalized();

   progressVelocity = progressVelocity * VELOCITY;

	//cout << "Up Vector : " << up.x << " " << up.y << " " << up.z << "\n";

	//cout << previousHeadUp.x << " " << previousHeadUp.y << " " << previousHeadUp.z << "\n";
	//assert(false);
	currentAngle = 180.0f / 3.14159265f * acos(previousHeadUp.Normalized().Dot(headUp.Normalized()));
	//cout << "Angle is : " << angle << "\n";
}

Vector3<float> Player::updateVelocity(float diffX, float diffY)
{
   //ASSUMES X AND Y PASSED IN ARE DIFFERENCES IN SDL SCREEN COORDS
   lastScreenX = diffX;
   lastScreenY = diffY;
}

Vector3<float> Player::getUp()
{
   return up;
}

Vector3<float> Player::getForward()
{
   return forward;
}

Vector3<float> Player::getProgress()
{
   return progress;
}

Vector3<float> Player::getPosition()
{
   return position;
}

void Player::setUp(Vector3<float> upVal)
{
   up = upVal;
}

void Player::calculateSide()
{
   side = up.Cross(progressVelocity);
   side = side.Normalized();
}

void Player::setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp, Vector3<float> prevHeadPos,
   Vector3<float> prevHeadUp) {
	up = prevHeadUp;
   previousHeadUp = prevHeadUp;
	previousHeadPos = prevHeadPos;
	currentHeadUp = currHeadUp;
	currentHeadPos = currHeadPos;
}

