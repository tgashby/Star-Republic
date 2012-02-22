#include "Camera.h"

Camera::Camera(PathPoint* head, PathPoint* tail)
   : m_pathPos(0,0,0), m_pathRef(0,0,1), m_pathUp(0,1,0), m_pathSide (1, 0, 0),
     m_eye(0,0,0), m_ref(0,0,1), m_up(0,1,0), cameraType(_MOTION_CAMERA), 
     m_turning(false), m_boosting(false), m_boostTime(0) {

   //Save the tail and head
   m_tail = tail;
   m_head = head;
   
   //Set the current up, position, reference, side, and forward vectors
   m_pathUp = m_tail->getUp();
   m_pathPos = tail->getPosition();
   m_pathRef = m_pathPos + ((head->getPosition() 
			     - tail->getPosition()).Normalized() 
			    * CAMERA_LOOK_AHEAD_DISTANCE);
   m_pathForw = (m_head->getPosition() - 
		 m_tail->getPosition()).Normalized();
   calculateSide();

   //Calculate the angle between them
   m_pathAngle = angleBetween(m_head->getUp(), m_tail->getUp());
   
   //Initialize the real look at vectors  
   m_eye = m_pathPos;
   m_ref = m_pathRef;
   m_up = m_pathUp;
}

Camera::~Camera() {

}

vec3 Camera::getPosition()
{
  return m_pathPos;
}

mat4 Camera::getProjectionViewMtx() {
   // Set the projection matrix
   float h = 4.0f * 640 / 480;
   mat4 projectionMatrix = mat4::Frustum(-h / 2, h / 2, -2, 2, 4, 6000);
   
   // Apply the view
   projectionMatrix = mat4::LookAt(projectionMatrix, m_eye, m_ref, m_up);

   return projectionMatrix;
}

void Camera::checkPath(PathPoint* head) {
  if (head != m_head) {
    m_tail = m_head;
    m_head = head;

    //Shouldn't need to change, but might have to
    m_pathUp = m_tail->getUp();
    calculateSide();

    m_pathAngle = angleBetween(m_head->getUp(), m_tail->getUp());
    
    m_turning = true;
  }
}

 void Camera::calculateSide() {
  m_pathSide = m_pathUp.Cross(m_pathRef - m_pathPos).Normalized();
}

void Camera::tic(uint64_t time) {
  vec3 temp1, temp2;
  vec3 tempVec;
   mat4 tempMatrix;
   float pathAccOffset = 0.0;

  // Find the angle between the two paths
//  float rotAngle = m_pathAngle * (m_head->getPosition() - m_pathPos).Length() 
//     / ((m_head->getPosition() - m_tail->getPosition()).Length() 
//     - CAMERA_LOOK_AHEAD_DISTANCE);

  // If we are turning our forward, but the angle between ourselves and 
  // the intended is close enough, stop turning to allow for rotation
  if (m_turning && .5 > 
      angleBetween(m_head->getPosition() - m_tail->getPosition(), 
		   m_pathRef - m_pathPos)) {
    m_turning = false;
    m_pathPos = m_pathRef - ((m_head->getPosition() 
			      - m_tail->getPosition()).Normalized() 
			     * CAMERA_LOOK_AHEAD_DISTANCE);
    m_pathUp = m_tail->getUp();
  }
  
  // If we aren't turning, its safe to rotate
  if (!m_turning) {
     temp1 = m_tail->getUp() * (m_head->getPosition() - m_pathRef).Length() / 
	(m_head->getPosition() - m_tail->getPosition()).Length();
     temp2 = m_head->getUp() * (m_pathRef - m_tail->getPosition()).Length() / 
	(m_head->getPosition() - m_tail->getPosition()).Length();
     m_pathUp = (temp1 + temp2).Normalized();
  } 

  // Move our reference point down the path
  if (m_boosting) {
     m_boostTime += time;
     if (m_boostTime > 2000) {
	m_boostTime = 2000;
     }

     //if (m_boostTime > 500) {
	pathAccOffset = (CAMERA_BOOST_ACC * m_boostTime);
	//}
	//else {
	//pathAccOffset = (CAMERA_BOOST_ACC * m_boostTime * -0.8);
	//}

     m_pathRef += (((m_head->getPosition() - m_pathRef).Normalized()) * 
		   ((time * CAMERA_DEF_VELOCITY) + pathAccOffset));
  }
  else {
     m_pathRef += (((m_head->getPosition() - m_pathRef).Normalized()) * 
		   (time * CAMERA_DEF_VELOCITY));
  }
  
  calculateSide();
  tempVec = (m_pathRef - m_pathPos).Normalized();
  m_pathPos = m_pathRef - (tempVec * CAMERA_LOOK_AHEAD_DISTANCE);

  setLookAt();
}

void Camera::setLookAt() {
   if (cameraType == _PATH_CAMERA) {
      m_eye = m_pathPos;
      m_ref = m_pathRef;
      m_up = m_pathUp;
      m_player->setVisible(true);
   }

   if (cameraType == _MOTION_CAMERA) {
      //DOESNT'T WORK PERFECTLY NOW BECAUSE OF CHANGES TO CAMERA/PLAYER
      m_eye = (m_player->getPosition() - 
	       (getForward() * CAMERA_DIST_FROM_PLAYER) + m_pathPos) / 2;
      //Pulls camera back for acceleration purposes
      //May want to make it be exponentially pulled back with time
      m_eye = m_eye - (getForward() * (.025 * m_boostTime));
      m_ref = m_pathRef;
      m_up = m_pathUp;
      m_player->setVisible(true);
   }
   
   if (cameraType == _SHIP_CAMERA) {
      m_eye = (m_player->getPosition() 
	       - (m_player->getAimForward() * CAMERA_DIST_FROM_PLAYER) 
	       + (m_player->getAimUp() * SHIP_CAMERA_ABOVE_SCALAR));
      m_ref = m_player->getAimForward() + m_player->getPosition();
      m_up = m_player->getAimUp();
      m_player->setVisible(true);
   }

   if (cameraType == _FPS_CAMERA) {
      m_eye = m_player->getPosition();
      m_ref = m_player->getAimForward() + m_player->getPosition();
      m_up = m_player->getAimUp();
      m_player->setVisible(false);
   }
}

float Camera::angleBetween(vec3 one, vec3 two) {
  return 180.0f / 3.14159265f *
    acos(one.Normalized().Dot(two.Normalized()));
}

vec3 Camera::getRef() {
   return m_pathRef;
}

vec3 Camera::getForward() {
   return (m_pathRef - m_pathPos).Normalized();
}

vec3 Camera::getUp() {
  return m_pathUp;
}

void Camera::setCameraType(int cam) {
   cameraType = cam;
}

void Camera::setPlayer(Player* player) {
   m_player = player;
}

void Camera::setBoosting(bool boostStatus) {
   m_boosting = boostStatus;
   if (boostStatus == false) {
      m_boostTime = 0;
   }
}

bool Camera::isBoosting() {
   return m_boosting;
}
