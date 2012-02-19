#include "EnemyGunship.h"

#include "Bullet.h"
#include <math.h>

#define VCHANGE 0.8
#define VINTENS 0.5
#define SIZE 40
#define MOTIONTIME 1000
#define FIRINGTIME 400
#define TURRETSIDEOFFSET 20
#define TURRETFORWARDOFFSET 20
#define TURRETUPOFFSET -16
#define _ENEMY_GUNSHIP_EXPLOSION_RADIUS 60.0f
const float PATHVELOCITY = 3.0f;
const float AIMVELOCITY = 0.003f;
const float x_SCALAR = 0.0005f; 
const float y_SCALAR = 0.0005f; 
const float mODEL_SCALE = 0.05f;
const float TURRETBASE_SCALE = 0.05f;
const float TURRETHEAD_SCALE = 0.1f;
const float ROTATE_CONSTANT = -90;

/** Gunner Enemy: Takes in the mesh info and a reference to the player to aim at **/
EnemyGunship::EnemyGunship(string fileName, string turretFileName1, 
			   string turretFileName2, string textureName, 
			   Modules *modules, Player &p) 
  : Flyer(), Enemy(p), 
    Explodeable(vec3(0,0,0), _ENEMY_GUNSHIP_EXPLOSION_RADIUS, modules), 
    side(1,0,0), currentAngle(0), prevAngle(0)
{
  m_mesh = new Mesh(fileName, textureName, modules);
  m_meshList.push_back(m_mesh);
  m_turretbasemesh1 = new Mesh(turretFileName1, textureName, modules);
  m_meshList.push_back(m_turretbasemesh1);
  m_turretheadmesh1 = new Mesh(turretFileName2, textureName, modules);
  m_meshList.push_back(m_turretheadmesh1);
  m_turretbasemesh2 = new Mesh(turretFileName1, textureName, modules);
  m_meshList.push_back(m_turretbasemesh2);
  m_turretheadmesh2 = new Mesh(turretFileName2, textureName, modules);
  m_meshList.push_back(m_turretheadmesh2);

  srandom(99901);
 
  /** aim vector **/
  dpos = (m_playerRef.getPosition() - m_position).Normalized();

  /** Setting the size of the bounding structure **/
  setBounds(SIZE);

  firingTimer1 = 0;
  firingTimer2 = FIRINGTIME / 2;

  motionTimer = 0;

  pxdir = 0;
  pydir = 0;
  xdir = (int) random()%3 - 1;
  ydir = (int) random()%3 - 1;

  /** setting the modelmatrix based on a constant scale and rotation,
   *  and the forward, up and position (aka Magic) **/
  mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
     mat4::Magic(-getForward(), getUp(), getPosition());
  m_mesh->setModelMtx(modelMtx);

  /** first turret **/
  mat4 modelMtx2 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(90, vec3(0,0,1)) *
     mat4::Translate(TURRETSIDEOFFSET + m_position.x, 
       TURRETUPOFFSET + m_position.y, TURRETFORWARDOFFSET + m_position.z);
  m_turretbasemesh1->setModelMtx(modelMtx2);

  /*mat4 modelMtx3 = mat4::Scale(TURRET_SCALE) * mat4::Rotate(90, vec3(0,0,1)) *
     mat4::Translate(TURRETSIDEOFFSET, 0, TURRETFORWARDOFFSET) * 
     mat4::Magic(-getAimForward(), getAimUp(), getPosition());
  m_turretheadmesh1->setModelMtx(modelMtx3);*/

  /** second turret **/

  mat4 modelMtx4 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(-90, vec3(0,0,1)) *
     mat4::Translate(-TURRETSIDEOFFSET + m_position.x, 
       TURRETUPOFFSET + m_position.y, TURRETFORWARDOFFSET + m_position.z);
  m_turretbasemesh2->setModelMtx(modelMtx4);

  m_health = 9001;
}

EnemyGunship::~EnemyGunship()
{
   delete m_mesh;
}


//All Vectors are updated in here
void EnemyGunship::tic(uint64_t time)
{
  if (isAlive()) {
    /** the normalized vector between the player and the enemy **/
    dpos = (m_playerRef.getPosition() - m_position).Normalized();
    
    // moving based on the player's direction
    m_position += m_playerRef.getForward() * PATHVELOCITY; 
    
    /** gunship AI -> movement first (the last term is there to simulate acceleration **/
    m_position += getUp() * ydir * AIMVELOCITY * 
      ((1 + MOTIONTIME / 2 - abs((float)motionTimer - (MOTIONTIME / 2)))); 
    m_position += getSide() * xdir * AIMVELOCITY * 
      ((1 + MOTIONTIME / 2 - abs((float)motionTimer - (MOTIONTIME / 2)))); 

    /** setting the modelmatrix based on a constant scale and rotation,
     *  and the forward, up and position (aka Magic) **/
    mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0));
    modelMtx *= mat4::Magic(getForward(), getAimUp(), getPosition());
    m_mesh->setModelMtx(modelMtx);

    /** first turret **/
    mat4 modelMtx2 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(90, vec3(0,0,1)) *
      mat4::Translate(TURRETSIDEOFFSET + m_position.x, 
		      TURRETUPOFFSET + m_position.y, TURRETFORWARDOFFSET + m_position.z);
    m_turretbasemesh1->setModelMtx(modelMtx2);
    
    /*mat4 modelMtx3 = mat4::Scale(TURRET_SCALE) * mat4::Rotate(90, vec3(0,0,1)) *
      mat4::Translate(TURRETSIDEOFFSET, 0, -TURRETFORWARDOFFSET) * 
      mat4::Magic(-getAimForward(), getAimUp(), getPosition());
      m_turretheadmesh1->setModelMtx(modelMtx3);*/
    

    /** second turret **/
    mat4 modelMtx4 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(-90, vec3(0,0,1)) *
      mat4::Translate(-TURRETSIDEOFFSET + m_position.x, 
		      TURRETUPOFFSET + m_position.y, TURRETFORWARDOFFSET + m_position.z);
    m_turretbasemesh2->setModelMtx(modelMtx4);
    
    /** update the timer for shooting **/
    firingTimer1 += time;
    firingTimer2 += time;

    /** to shoot? **/
    if (firingTimer1 > FIRINGTIME && 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef.getForward())) > 90)
      {
	firing1 = true;
	firingTimer1 %= FIRINGTIME;
      }
    else 
      {
	firing1 = false;
      }
    firing1 = firing1 && isAlive();
    
    /** to shoot? **/
    if (firingTimer2 > FIRINGTIME && 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef.getForward())) > 90)
      {
	firing2 = true;
	firingTimer2 %= FIRINGTIME;
      }
    else 
      {
	firing2 = false;
      }
    firing2 = firing2 && isAlive();

    /** every so often, change direction of motion **/
    motionTimer += time;
    if (motionTimer > MOTIONTIME)
      {
	/** If the player is currently moving, store the direction
	 *  so we can avoid moving repeatedly in the same direction **/
	if (xdir != 0)
	  pxdir = xdir;
	if (ydir != 0)
	  pydir = ydir;
	
	/** select a new direction (i.e. one that hasn't been chosen recently) **/
	do
	  xdir = (int) random()%3 - 1;
	while (xdir == pxdir);
	
	do
	  ydir = (int) random()%3 - 1;
	while (ydir == pydir);
	
	motionTimer = 0;
      }
  }
  else {
    explosionTic(time);
  }
}

/** see: Player **/
void EnemyGunship::setBearing(Vector3<float> headPosition, Vector3<float> headUp)
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

/** The aim direction is the normalized vector between the enemy's
 *  and the player's position **/
Vector3<float> EnemyGunship::getAimForward()
{
   return dpos;
}
/** The up vector is based on parent class, nuff said **/
Vector3<float> EnemyGunship::getAimUp()
{
   return getUp();
}


/** returns the side vector as computeSide() would **/
Vector3<float> EnemyGunship::getSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
   return side;
}

/** Return the location of the right-arm cannon for shooting purposes **/
Vector3<float> EnemyGunship::getLeftCannonPos()
{
   return m_position + getAimForward().Cross(getAimUp()) * 10;
}
/** Return the location of the left-arm cannon for shooting purposes **/
Vector3<float> EnemyGunship::getRightCannonPos()
{
   return m_position - getAimForward().Cross(getAimUp()) * 10;
}

/** computes (but does not return) the actual side vector **/
void EnemyGunship::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}

/** Oh shiiiiii... do the collision! You lose 25 health. You may die. **/
void EnemyGunship::doCollision(GameObject & other)
{
   if (typeid(other) == typeid(Bullet))
   {
      if (&((Bullet&)other).getParent() != this) 
      {
         m_health -= 25;
      }
   }
   if (typeid(other) == typeid(Missile)) {
     m_health -= 100;
   }
   
   if (m_health <= 0) 
     {
       m_alive = false;
       m_mesh->setVisible(false);
       setExplosionPosition(m_position);
     }
}
/** to fire or not to fire? **/
bool EnemyGunship::shouldFire1()
{
   return firing1;
}
/** to fire or not to fire? **/
bool EnemyGunship::shouldFire2()
{
   return firing2;
}

vec3 EnemyGunship::getPosition() {
  return m_position;
}
