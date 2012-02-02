#ifndef StarRepub_Camera_h
#define StarRepub_Camera_h

#include "../Interfaces.h"

class Camera: public ICamera {
public:
   Camera(vec3 start);
   ~Camera();
   mat4 getProjectionViewMtx();
   void rotLocal(float pitch, float yaw);
   void moveInOut(float dist);
private:
   vec3 m_eye;
   vec3 m_ref;
   vec3 m_up;
   vec3 m_fwd;
   float m_pitch, m_yaw, m_dist;
};

#endif