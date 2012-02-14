#include "GameObject.h"
#include "Mesh.h"

#define DEFAULT_MESH "spaceship.obj"
#define DEFAULT_TEXTURE "blank.bmp"

GameObject::GameObject(vec3 startPos, vec3 startVelocity, 
                       vec3 forwardVec, vec3 accelerationVec, 
                       vec3 upVec, float collideRadius, int health)
{
   m_position = startPos;
   m_velocity = startVelocity;
   m_forward = forwardVec;
   m_acceleration = accelerationVec;
   m_up = upVec;
   m_boundingSphere = new BoundingSphere(collideRadius); 
   m_health = health;
   
   m_alive = true;
}

GameObject::GameObject()
{
   m_position = vec3(0,0,0);
   m_velocity = vec3(0,0,0);
   m_forward = vec3(0,0,-1);
   m_acceleration = vec3(0,0,0);
   m_up = vec3(0,1,0);
   m_boundingSphere = new BoundingSphere(defaultBoundingRadius);
   m_health = 100;
   
   m_alive = true;
}


GameObject::~GameObject()
{
   delete m_boundingSphere;
}

void GameObject::tic(uint64_t td)
{
   
}

void GameObject::setBounds(float f)
{
   m_boundingSphere = new BoundingSphere(f); 
}

void GameObject::setVelocity(const Vector3<float> velocity)
{
   m_velocity = velocity;
}

void GameObject::setUp(Vector3<float> upVal)
{
   m_up = upVal;
}

void GameObject::setPosition(Vector3<float> pos)
{
   m_position = pos;
}

void GameObject::setAcceleration(Vector3<float> acc)
{
   m_acceleration = acc;
}

void GameObject::setForward(Vector3<float> forward)
{
   m_forward = forward;
}

const Vector3<float> GameObject::getPosition() const
{
   return m_position;
}

const Vector3<float> GameObject::getForward() const
{
   return m_forward;
}

const Vector3<float> GameObject::getUp() const
{
   return m_up;
}

const BoundingSphere * GameObject::getBoundingSphere() const
{
   return m_boundingSphere;
}

bool GameObject::collidesWith (const GameObject & other) const {
   return m_boundingSphere->collidesWith(other.getBoundingSphere(),m_position,
                                         other.getPosition()); 
}

bool GameObject::isAlive()
{
   return m_alive;
}