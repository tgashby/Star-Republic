#include "Player.h"

Player::Player(string fileName, string textureName, Modules *modules) 
   : Object3d(), velocity(0,0,0), position(0,0,0), acceleration(0,0,0), health(100)
{
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx = mat4::Translate(0, 0, -100);
   modelMtx = mat4::Rotate(180, vec3(0, 1, 0)) * modelMtx;

   m_mesh->setModelMtx(modelMtx);
}
Player::~Player()
{

}
void Player::tic(unsigned int time)
{
   velocity.x += acceleration.x * time;
   velocity.y += acceleration.y * time;
   velocity.z += acceleration.z * time;

   position.x += velocity.x * time;
   position.y += velocity.y * time;
   position.z += velocity.z * time;
}
void Player::setPosition(Vector3<float> pos)
{
   position.x = pos.x;
   position.y = pos.y;
   position.z = pos.z;
}
void Player::setVelocity(Vector3<float> vel)
{
   velocity.x = vel.x;
   velocity.y = vel.y;
   velocity.z = vel.z;
}
void Player::setAcceleration(Vector3<float> acc)
{
   acceleration.x = acc.x;
   acceleration.y = acc.y;
   acceleration.z = acc.z;
}
void Player::setBearing(Vector3<float> current)
{
   velocity.x = current.x - position.x;
   velocity.y = current.y - position.y;
   velocity.z = current.z - position.z;
   velocity->normalized();
   velocity = velocity * VELOCITY;
}
Vector3<float> Player::getPosition()
{
   return position;
}
