#include "Camera.h"

Camera::Camera(WorldPoint* head, WorldPoint* tail)
  : m_eye(0,0,0), m_ref(0,0,1), m_up(0,1,0), m_side (1, 0, 0) {
  m_tail = tail;
  m_head = head;
  
  m_up = m_tail->getUp();
  

  m_eye = tail->getPosition();
  m_ref = m_eye + ((head->getPosition() - tail->getPosition()).Normalized() * CAMERA_LOOK_AHEAD_DISTANCE);
  m_forw = (m_head->getPosition() - m_tail->getPosition()).Normalized();
  calculateSide();

  m_pathAngle = angleBetween(m_head->getUp(), m_tail->getUp());

  m_turning = false;

}

Camera::~Camera() {

}

vec3 Camera::getPosition()
{
  return m_eye;
}

mat4 Camera::getProjectionViewMtx() {
   // Set the projection matrix
   float h = 4.0f * 640 / 480;
   mat4 projectionMatrix = mat4::Frustum(-h / 2, h / 2, -2, 2, 4, 2000);
   
   // apply the view
   projectionMatrix = mat4::LookAt(projectionMatrix, m_eye, m_ref, m_up);
   cout << "Camera eye is " << m_eye.x << " " << m_eye.y << " " << m_eye.z << "\n";
   return projectionMatrix;
}

void Camera::checkPath(WorldPoint* head) {
  if (head != m_head) {
    m_tail = m_head;
    m_head = head;

    m_up = m_tail->getUp();
    calculateSide();

    m_pathAngle = angleBetween(m_head->getUp(), m_tail->getUp());
    
    m_turning = true;
  }
}

void Camera::calculateSide() {
  m_side = m_up.Cross(m_ref - m_eye).Normalized();
}

void Camera::tic(uint64_t time) {
  vec3 temp1, temp2;
  vec3 tempVec;
  mat4 tempMatrix;

  float rotAngle = m_pathAngle * (m_head->getPosition() - m_eye).Length() / 
    ((m_head->getPosition() - m_tail->getPosition()).Length() - CAMERA_LOOK_AHEAD_DISTANCE);

  if (m_turning && .5 > angleBetween(m_head->getPosition() - m_tail->getPosition(), 
				     m_ref - m_eye)) {
    m_turning = false;
    m_eye = m_ref - ((m_head->getPosition() - m_tail->getPosition()).Normalized() 
		     * CAMERA_LOOK_AHEAD_DISTANCE);
    //m_up = m_tail->getUp();
  }
  
  if (!m_turning) {
  temp1 = m_head->getUp() * (m_head->getPosition() - m_ref).Length() / 
     (m_head->getPosition() - m_tail->getPosition()).Length();
  temp2 = m_head->getUp() * (m_ref - m_tail->getPosition()).Length() / 
     (m_head->getPosition() - m_tail->getPosition()).Length();
  m_up = (temp1 + temp2).Normalized();
  m_up = vec3(0, 1, 0);
    //ADD forward between them
  } 

  m_ref += (((m_head->getPosition() - m_ref).Normalized()) * 
	    (time * CAMERA_REF_VELOCITY));
  
  calculateSide();
  tempVec = (m_ref - m_eye).Normalized();
  m_eye = m_ref - (tempVec * CAMERA_LOOK_AHEAD_DISTANCE);
}

float Camera::angleBetween(vec3 one, vec3 two) {
  return 180.0f / 3.14159265f *
    acos(one.Normalized().Dot(two.Normalized()));
}

vec3 Camera::getRef() {
   return m_ref;
}

vec3 Camera::getForward() {
   return (getRef() - getPosition()).Normalized();
}

vec3 Camera::getUp() {
  return m_up;
}
