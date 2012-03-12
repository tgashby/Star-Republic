#include "EnemyGunship.h"

#include "Bullet.h"
#include <math.h>

#define VCHANGE 0.8
#define VINTENS 0.5
#define SIZE 100
#define MOTIONTIME 2000
#define FIRINGTIME 400
#define TURRETSIDEOFFSET 20
#define TURRETFORWARDOFFSET -25
#define TURRETUPOFFSET 0
#define _ENEMY_GUNSHIP_EXPLOSION_RADIUS 60.0f
#define UPDATEDISTANCE 2000.0
#define RADTODEG 180 / 3.1415
const float PATHVELOCITY = 0.03f;
const float AIMVELOCITY = 0.00016f;
const float x_SCALAR = 0.0005f; 
const float y_SCALAR = 0.0005f; 
const float mODEL_SCALE = 0.05f;
const float TURRETBASE_SCALE = 0.05f;
const float TURRETHEAD_SCALE = 0.2f;
const float ROTATE_CONSTANT = -90;

/** Gunner Enemy: Takes in the mesh info and a reference to the player to aim at **/
EnemyGunship::EnemyGunship(string fileName, string turretFileName1,
			   string turretFileName2, string bodyTextureName,
                           string baseTextureName, string headTextureName, 
			   Modules *modules, Player *p) 
  : Flyer(), Enemy(p), 
    Explodeable(vec3(0,0,0), _ENEMY_GUNSHIP_EXPLOSION_RADIUS, modules), 
    side(1,0,0), currentAngle(0), prevAngle(0)
{
  m_mesh = new Mesh(fileName, bodyTextureName, modules);
  m_meshList.push_back(m_mesh);
  m_turretbasemesh1 = new Mesh(turretFileName1, baseTextureName, modules);
  m_meshList.push_back(m_turretbasemesh1);
  m_turretheadmesh1 = new Mesh(turretFileName2, headTextureName, modules);
  m_meshList.push_back(m_turretheadmesh1);
  m_turretbasemesh2 = new Mesh(turretFileName1, baseTextureName, modules);
  m_meshList.push_back(m_turretbasemesh2);
  m_turretheadmesh2 = new Mesh(turretFileName2, headTextureName, modules);
  m_meshList.push_back(m_turretheadmesh2);
 
  /** aim vector **/
  dpos = (m_playerRef->getPosition() - m_position).Normalized();

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
     mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1)) * mat4::Magic(-getForward(), getUp(), getPosition());
  m_mesh->setModelMtx(modelMtx);

  /** first turret **/
  mat4 modelMtx2 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(90, vec3(0,0,1)) *
     mat4::Translate(TURRETSIDEOFFSET + m_position.x, 
       TURRETUPOFFSET + m_position.y, TURRETFORWARDOFFSET + m_position.z);
  m_turretbasemesh1->setModelMtx(modelMtx2);

  /** second turret **/
  mat4 modelMtx4 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
     mat4::Translate(-TURRETSIDEOFFSET + m_position.x, 
       TURRETUPOFFSET + m_position.y, TURRETFORWARDOFFSET + m_position.z);
  m_turretbasemesh2->setModelMtx(modelMtx4);

  m_health = 20;
}

EnemyGunship::~EnemyGunship()
{
   delete m_mesh;
}


//All Vectors are updated in here
void EnemyGunship::tic(uint64_t time)
{
  dpos = (m_playerRef->getPosition() - m_position);
  if (isAlive() && (dpos.Length() < UPDATEDISTANCE)) {
    /** the normalized vector between the player and the enemy **/
    dpos = dpos.Normalized();
    
    // moving based on the player's direction
    m_forward = m_playerRef->getMForward();
    m_up = m_playerRef->getAimUp();
    
    /** gunship AI -> movement first (the last term is there to simulate acceleration) **/
    m_position += m_forward * PATHVELOCITY * time;
    m_position += m_up * ydir * AIMVELOCITY * 
      ((1 + MOTIONTIME / 2 - abs((float)motionTimer - (MOTIONTIME / 2)))) * time; 
    m_position += getMSide() * xdir * AIMVELOCITY * 
      ((1 + MOTIONTIME / 2 - abs((float)motionTimer - (MOTIONTIME / 2)))) * time; 

    /** setting the modelmatrix based on a constant scale and rotation,
     *  and the forward, up and position (aka Magic) **/
    mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
                    mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1));
    modelMtx *= mat4::Magic(m_forward, m_up, m_position);
    m_mesh->setModelMtx(modelMtx);

    /** first turret **/
    mat4 modelMtx2 =  mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
       mat4::Translate(TURRETSIDEOFFSET, 0, TURRETFORWARDOFFSET) *
       mat4::Magic(m_forward, m_up, m_position);
    m_turretbasemesh1->setModelMtx(modelMtx2);
    
    vec3 tpos1 = vec3(m_position.x, m_position.y, m_position.z);
    tpos1 += getMSide() * TURRETSIDEOFFSET;
    tpos1 += m_forward * (TURRETFORWARDOFFSET + 10.0);
    tpos1 += m_up * (TURRETUPOFFSET + 2.0);
    mat4 modelMtx3 = mat4::Scale(TURRETHEAD_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
       mat4::Magic(-getAimForward(), getAimUp(), tpos1);
    m_turretheadmesh1->setModelMtx(modelMtx3);

    /** second turret **/
    mat4 modelMtx4 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(90, vec3(0,1,0)) *
       mat4::Translate(-TURRETSIDEOFFSET, 0, TURRETFORWARDOFFSET) *
       mat4::Magic(m_forward, m_up, m_position);
    m_turretbasemesh2->setModelMtx(modelMtx4);

    vec3 tpos2 = vec3(m_position.x, m_position.y, m_position.z);
    tpos2 -= getMSide() * TURRETSIDEOFFSET;
    tpos2 += m_forward * (TURRETFORWARDOFFSET + 10.0);
    tpos2 += m_up * (TURRETUPOFFSET + 2.0);
    mat4 modelMtx5 = mat4::Scale(TURRETHEAD_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
       mat4::Magic(-getAimForward(), getAimUp(), tpos2);
    m_turretheadmesh2->setModelMtx(modelMtx5);
    
    /** update the timer for shooting **/
    firingTimer1 += time;
    firingTimer2 += time;

    /** to shoot? **/
    if (firingTimer1 > FIRINGTIME && 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef->getForward())) > 90)
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
    if (firingTimer2 > FIRINGTIME && 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef->getForward())) > 90)
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
  else if (dpos.Length() < UPDATEDISTANCE) {
    explosionTic(time);
  }
  else if (isAlive())
  {
     dpos = dpos.Normalized();
    
    // moving based on the player's direction
    m_forward = m_playerRef->getMForward();
    m_up = m_playerRef->getAimUp();

     mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
                    mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1));
    modelMtx *= mat4::Magic(m_forward, m_up, m_position);
    m_mesh->setModelMtx(modelMtx);
    m_mesh->setVisible(true);

    /** first turret **/
    mat4 modelMtx2 =  mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
       mat4::Translate(TURRETSIDEOFFSET, 0, TURRETFORWARDOFFSET) *
       mat4::Magic(m_forward, m_up, m_position);
    m_turretbasemesh1->setModelMtx(modelMtx2);
    m_turretbasemesh1->setVisible(true);
    
    
    vec3 tpos1 = vec3(m_position.x, m_position.y, m_position.z);
    tpos1 += getMSide() * TURRETSIDEOFFSET;
    tpos1 += m_forward * (TURRETFORWARDOFFSET + 10.0);
    tpos1 += m_up * (TURRETUPOFFSET + 2.0);
    mat4 modelMtx3 = mat4::Scale(TURRETHEAD_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
       mat4::Magic(-getAimForward(), getAimUp(), tpos1);
    m_turretheadmesh1->setModelMtx(modelMtx3);
    m_turretheadmesh1->setVisible(true);


    /** second turret **/
    mat4 modelMtx4 = mat4::Scale(TURRETBASE_SCALE) * mat4::Rotate(90, vec3(0,1,0)) *
       mat4::Translate(-TURRETSIDEOFFSET, 0, TURRETFORWARDOFFSET) *
       mat4::Magic(m_forward, m_up, m_position);
    m_turretbasemesh2->setModelMtx(modelMtx4);
    m_turretbasemesh2->setVisible(true);

    vec3 tpos2 = vec3(m_position.x, m_position.y, m_position.z);
    tpos2 -= getMSide() * TURRETSIDEOFFSET;
    tpos2 += m_forward * (TURRETFORWARDOFFSET + 10.0);
    tpos2 += m_up * (TURRETUPOFFSET + 2.0);
    mat4 modelMtx5 = mat4::Scale(TURRETHEAD_SCALE) * mat4::Rotate(-90, vec3(0,1,0)) *
       mat4::Magic(-getAimForward(), getAimUp(), tpos2);
    m_turretheadmesh2->setModelMtx(modelMtx5);
    m_turretheadmesh2->setVisible(true);

    
    m_position = spawnpos;
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
   return m_position + getAimForward().Cross(getAimUp()) * 18;
}
/** Return the location of the left-arm cannon for shooting purposes **/
Vector3<float> EnemyGunship::getRightCannonPos()
{
   return m_position - getAimForward().Cross(getAimUp()) * 18;
}

/** computes (but does not return) the actual side vector **/
void EnemyGunship::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}

void EnemyGunship::setSpawnPosition(vec3 position)
{
   m_position = position;
   spawnpos = getPosition();
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
       m_turretbasemesh1->setVisible(false);
       m_turretheadmesh1->setVisible(false);
       m_turretbasemesh2->setVisible(false);
       m_turretheadmesh2->setVisible(false);
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

vec3 EnemyGunship::getMSide()
{
  return m_forward.Cross(m_up);
}
