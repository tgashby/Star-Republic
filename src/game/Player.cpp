#include "Player.h"

Player::Player(string fileName, string textureName, Modules *modules) 
//   : Object3d(fileName, textureName, modules), velocity(0,0,0), position(0,0,0), acceleration(0,0,0), health(100)
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
   
   //MAYBE NOT THE BEST WAY TO DO IT
   mat4 modelMtx = m_mesh->getModelMtx();
   modelMtx = mat4::Translate(velocity.x * time, velocity.y * time, velocity.z * time) * modelMtx;
   m_mesh->setModelMtx(modelMtx);

   cerr << "WE BE AT " << position.x << " " << position.y << " " << position.z << " CAP'N!\n";
   cerr << "OUR SPEED: X IS: " << velocity.x << ", Y IS: " << velocity.y << ", Z IS: " << velocity.z << "\n";
}

void Player::setPosition(Vector3<float> pos)
{
   position.x = pos.x;
   position.y = pos.y;
   position.z = pos.z;
   cerr << "WE BE AT " << position.x << " " << position.y << " " << position.z << " CAP'N!\n";
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
   cerr << "OUR HEADING: X IS: " << current.x << ", Y IS: " << current.y << ", Z IS: " << current.z << "\n";

   velocity.x = current.x - position.x;
   velocity.y = current.y - position.y;
   velocity.z = current.z - position.z;

   velocity = velocity.Normalized();

   forward.x = velocity.x;
   forward.y = velocity.y;
   forward.z = velocity.z;

   velocity = velocity * VELOCITY;
}

Vector3<float> Player::getPosition()
{
   return position;
}
