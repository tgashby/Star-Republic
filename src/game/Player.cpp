#include "Player.h"

Player::Player(string fileName, string textureName, Modules *modules) 
   : Object3d(), progressVelocity(0,0,0), progress(0,0,0), acceleration(0,0,0), health(100), 
     position(0,0,0), up(0,1,0), side(1,0,0), shipVelocity(0,0,0), lastScreenX(0),
     lastScreenY(0)
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
   progressVelocity.x += acceleration.x * time;
   progressVelocity.y += acceleration.y * time;
   progressVelocity.z += acceleration.z * time;

   shipVelocity = (((side * lastScreenX)) - (position - progress)) * 0.0005 + (((up * lastScreenY)) - (position - progress)) * 0.0005 + progressVelocity;

   progress.x += progressVelocity.x * time;
   progress.y += progressVelocity.y * time;
   progress.z += progressVelocity.z * time;

   //shipVelocity = (up * lastScreenY) + (side * lastScreenX) + progressVelocity;
   position = position + (shipVelocity * time);
   
   //MAYBE NOT THE BEST WAY TO DO IT
   mat4 modelMtx = m_mesh->getModelMtx();
   modelMtx = mat4::Translate(shipVelocity.x * time, shipVelocity.y * time,
      shipVelocity.z * time) * modelMtx;
   m_mesh->setModelMtx(modelMtx);
}

void Player::setProgress(Vector3<float> pos)
{
   progress.x = pos.x;
   progress.y = pos.y;
   progress.z = pos.z;
   cerr << "WE BE AT " << progress.x << " " << progress.y << " " << progress.z << " CAP'N!\n";
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

void Player::setBearing(Vector3<float> current)
{
   //cerr << "OUR HEADING: X IS: " << current.x << ", Y IS: " << current.y << ", Z IS: " << current.z << "\n";

   progressVelocity.x = current.x - progress.x;
   progressVelocity.y = current.y - progress.y;
   progressVelocity.z = current.z - progress.z;

   progressVelocity = progressVelocity.Normalized();

   progressVelocity = progressVelocity * VELOCITY;
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
