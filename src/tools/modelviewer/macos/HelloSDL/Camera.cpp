#include "Camera.h"
#include <iostream>

#define DEF_DISTANCE 40

Camera::Camera(vec3 start) {
   m_dist = DEF_DISTANCE;
   fwd = vec3(0, 0, m_dist);
   eye = start + fwd;
   ref = start;
   up = vec3(0, 1, 0);
   m_pitch = 0;
   m_yaw = 0;
}

Camera::~Camera() {
   
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
   fwd = vec3(d.x, d.y, d.z);
   eye = ref + fwd;
   /*
   mat4 rot = mat4::Rotate(m_pitch, vec3(1,0,0));
   rot = rot * mat4::Rotate(m_yaw, vec3(0,1,0));
   vec4 d = rot.TranslatePoint(vec4(0, 0, 1, 0));
   fwd = vec3(d.x, d.y, d.z);
   ref = eye + fwd;*/
}
