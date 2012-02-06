#include "Camera.h"

#define DEF_DISTANCE 40

Camera::Camera(vec3 start) {
   m_dist = DEF_DISTANCE;
   m_fwd = vec3(0, 0, m_dist);
   m_eye = start + m_fwd;
   m_ref = start;
   m_up = vec3(0, 1, 0);
   m_pitch = 0;
   m_yaw = 0;
}

Camera::~Camera() {
   
}

mat4 Camera::getProjectionViewMtx() {
   // Set the projection matrix
   float h = 4.0f * 640 / 480;
   mat4 projectionMatrix = mat4::Frustum(-h / 2, h / 2, -2, 2, 4, 200);
   
   // apply the view
   projectionMatrix = mat4::LookAt(projectionMatrix, m_eye, m_ref, m_up);
   
   return projectionMatrix;
}

//Chad and Nick's handiwork. Code is extremely volatile: do not uncomment unless you are prepared!
//Current camera will only handle following it directly
void Camera::update(vec3 playerPos, vec3 playerForw, vec3 playerUp) {
   m_ref = playerPos;
   assert(playerForw.x == playerForw.x);
   assert(playerForw.y == playerForw.y);
   assert(playerForw.z == playerForw.z);
   m_eye = playerPos - (playerForw * CAMERA_DIST_FROM_PLAYER);
   m_up = playerUp;
   cerr << "The UP is X: " << m_up.x << ", Y: " << m_up.y << ", Z: " << m_up.z << "\n";
}

void Camera::moveInOut(float dist) {
   m_dist += dist * 2;
   if (m_dist < 1.0) {
      m_dist = 1;
   }
   rotLocal(0, 0);
}

void Camera::rotLocal(float pitch, float yaw) {
   m_pitch += pitch;
   m_yaw += yaw;
   
   if (m_pitch > 80)
      m_pitch = 80;
   else if (m_pitch < -80)
      m_pitch = -80;
   
   if (m_yaw >= 360)
      m_yaw -= 360;
   else if (m_yaw < 0)
      m_yaw += 360;
   
   mat4 rot = mat4::Rotate(-m_pitch, vec3(1,0,0));
   rot = rot * mat4::Rotate(m_yaw, vec3(0,1,0));
   vec4 d = rot.TranslatePoint(vec4(0, 0, m_dist, 0));
   m_fwd = vec3(d.x, d.y, d.z);
   m_eye = m_ref + m_fwd;
}
