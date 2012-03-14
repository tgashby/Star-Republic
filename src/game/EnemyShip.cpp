#include "EnemyShip.h"
#include "Bullet.h"

#define VCHANGE 0.8
#define VINTENS 0.5
#define SCREENX 400
#define SCREENY 300
#define MAXDISTANCE 1200
#define MAXSCAREDANGLE 20
#define MAXSCAREDSPEED 0.35
#define DISTANCESCALE 100
#define DODGETIME 50
#define SIZE 40
#define EXPLOSION_SIZE 40
#define LODDISTANCE 2500

/** Scared Enemy: Takes in the mesh info and a reference to the player to aim at **/
EnemyShip::EnemyShip(string fileName, string LODname, string textureName, Modules *modules, Player *p) 
  :  Explodeable(vec3(0,0,0), EXPLOSION_SIZE, modules), 
     Flyer(), Enemy(p), side(1,0,0), currentAngle(0), prevAngle(0)
{
  m_mesh = new Mesh(fileName, textureName, modules);
  m_meshList.push_back(m_mesh);
  m_LODmesh = new Mesh(LODname, textureName, modules);
  m_meshList.push_back(m_LODmesh);

  /** aim vector **/
  dpos = (m_playerRef->getPosition() - m_position).Normalized();

  /** Setting the size of the bounding structure **/
  setBounds(SIZE);

  /** whether he's dodging **/
  dodging = false;
  /** what direction to dodge in: 
   * 1 for left of the player, -1 for right, (0 for not dodging) **/
  dodgedir = 0;
  /** how long he's been dodging for (he won't dodge permanently)**/
  dodgecounter = DODGETIME;

  /** setting the modelmatrix based on a constant scale and rotation,
   *  and the forward, up and position (aka Magic) **/
  mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
     mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1)) *
     mat4::Magic(-getForward(), getUp(), getPosition());
  m_mesh->setModelMtx(modelMtx);
  m_LODmesh->setModelMtx(modelMtx);

  m_mesh->setVisible(false);
  m_LODmesh->setVisible(true);

  flashtimer = 0;
  m_health = 150;

}

EnemyShip::~EnemyShip()
{
   delete m_mesh;
}


//All Vectors are updated in here
void EnemyShip::tic(uint64_t time)
{
  if (isAlive())
  {
    if ((m_playerRef->getPosition() - m_position).Length() > LODDISTANCE)
    {
       if (flashtimer != 0)
       {
          m_LODmesh->setVisible(false);
          m_mesh->setVisible(false);
          flashtimer--;
       }
       else
       {
          m_mesh->setVisible(false);
          m_LODmesh->setVisible(true);
       }
    }
    else
    {
       if (flashtimer != 0)
       {
          m_LODmesh->setVisible(false);
          m_mesh->setVisible(false);
          flashtimer--;
       }
       else
       {
          m_mesh->setVisible(true);
          m_LODmesh->setVisible(false);
       }
    }
  }

  if (time == 0)
  {
     mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
                    mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1));
     modelMtx *= mat4::Magic(getAimForward(), getAimUp(), getPosition());
     m_mesh->setModelMtx(modelMtx);
     m_LODmesh->setModelMtx(modelMtx);
     m_LODmesh->setVisible(true);
  }

  //printf("WT");

  dpos = (m_playerRef->getPosition() - m_position);
  if (isAlive() && dpos.Length() < MAXDISTANCE) {
    /** the normalized vector between the player and the enemy **/
    dpos = dpos.Normalized();

    // moving based on the player's direction and it's aiming direction
    m_position += m_playerRef->getMForward() * PATHVELOCITY * time + getAimForward() * AIMVELOCITY * time; 
    
    /** 'scared ship' AI **/
    float aimAngle;
    float distance;
    /** the angle between the player's aim and the enemy's location:
     *  If the player is aiming near the enemy, it dodges **/
    aimAngle = 180 - 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef->getAimForward()));
    distance = (float)((m_playerRef->getPosition() - m_position).Length());
    if (aimAngle < MAXSCAREDANGLE)
      {
	/** If this is the first tic he needs to dodge, compute the dodge direction **/
	if (!dodging)
	  {
	    if (180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef->getSide())) < 90)
	      dodgedir = 1;
	    else
	      dodgedir = -1;
	  }
	/** if he's dodged long enough, but is still being aimed at, 
	 *  dodge in the opposite direction **/
	if (dodgecounter == 0)
	  {
	    dodgecounter = DODGETIME;
	    dodgedir = -dodgedir;
	  }
	/** which direction to dodge in? **/
	vec3 motionDir = getScaredSide() / (aimAngle / MAXSCAREDANGLE) * MAXSCAREDSPEED * dodgedir *
	  ((DISTANCESCALE / distance) * (DISTANCESCALE / distance));
	/** move and dodge **/
	m_position += motionDir * time;
	dodging = true;
	/** count down - you can't dodge in the same direction forever **/
	dodgecounter--;
      }
    else
      {
	/** no longer being aimed at, dodging unnecessary **/
	dodging = false;
	dodgedir = 0;
	dodgecounter = DODGETIME;
      }

    /** setting the modelmatrix based on a constant scale and rotation,
     *  and the forward, up and position (aka Magic) - note that 
     *  the enemy constantly aims at the player **/
    mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
                    mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1));
    modelMtx *= mat4::Magic(getAimForward(), getAimUp(), getPosition());
    m_mesh->setModelMtx(modelMtx);
    m_LODmesh->setModelMtx(modelMtx);
    
    /** update the timer for shooting **/
    firingTimer += time;
    
    /** if it's time to shoot, let loose the cannons! (provided you're not behing the player) **/
    if (firingTimer > 600 && 180.0f / 3.14159265f * acos(dpos.Dot(m_playerRef->getForward())) > 80)
      {
	firing = true;
	firingTimer = 0;
      }
    else 
      {
	firing = false;
      }
    
    firing = firing && isAlive();
  }
  else if (isAlive())
  {
     m_up = m_playerRef->getAimUp();

     mat4 modelMtx = mat4::Scale(mODEL_SCALE) * mat4::Rotate(ROTATE_CONSTANT, vec3(0,1,0)) *
                    mat4::Rotate(ROTATE_CONSTANT, vec3(0,0,1));
     modelMtx *= mat4::Magic(getAimForward(), getAimUp(), getPosition());
     m_mesh->setModelMtx(modelMtx);
     m_LODmesh->setModelMtx(modelMtx);

     m_position = spawnpos;
  }
  else
  {
    explosionTic(time);
  }
}

/** see: Player **/
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

/** The aim direction is the normalized vector between the enemy's
 *  and the player's position **/
Vector3<float> EnemyShip::getAimForward()
{
   return dpos;
}
/** The up vector is based on parent class, nuff said **/
Vector3<float> EnemyShip::getAimUp()
{
   return getUp();
}

/** gets the side vector based on where the enemy is aiming 
 *  (rather than where he's moving) and the up vector **/
Vector3<float> EnemyShip::getScaredSide()
{
   vec3 aaargh = m_playerRef->getAimForward().Cross(m_playerRef->getAimUp()).Normalized();
   return aaargh;
}

void EnemyShip::setSpawnPosition(vec3 position)
{
   m_position = position;
   spawnpos = getPosition();
}

/** returns the side vector as computeSide() would **/
Vector3<float> EnemyShip::getSide()
{
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
   return side;
}

/** Return the location of the right-arm cannon for shooting purposes **/
Vector3<float> EnemyShip::getLeftCannonPos()
{
   return m_position + getAimForward().Cross(getAimUp()) * 10;
}
/** Return the location of the left-arm cannon for shooting purposes **/
Vector3<float> EnemyShip::getRightCannonPos()
{
   return m_position - getAimForward().Cross(getAimUp()) * 10;
}

/** computes (but does not return) the actual side vector **/
void EnemyShip::calculateSide() {
   side = m_up.Cross(m_currentHeadPos - m_previousHeadPos).Normalized();
}

/** Oh shiiiiii... do the collision! You lose 25 health. You may die. **/
void EnemyShip::doCollision(GameObject & other)
{
   if (typeid(other) == typeid(Bullet))
   {
      if (&((Bullet&)other).getParent() != this) 
      {
         m_health -= 25; 
         if (flashtimer == 0)
            flashtimer = 2;  
      }
   }

   if (typeid(other) == typeid(Missile)) {
     m_health -= 100;
     if (flashtimer == 0)
            flashtimer = 2;   
   }
   
   if (m_health <= 0) 
     {
       m_alive = false;
       m_mesh->setVisible(false);
       m_LODmesh->setVisible(false);
       setExplosionPosition(m_position);
     }
}
/** to fire or not to fire? **/
bool EnemyShip::shouldFire()
{
   return firing;
}

vec3 EnemyShip::getPosition() {
  return m_position;
}
