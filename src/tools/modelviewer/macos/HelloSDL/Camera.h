#ifndef SimpleGame_Camera_h
#define SimpleGame_Camera_h

#define FLOOR_OFFSET 4.5

#include "Interfaces.h"
#include "Object3d.h"

class Camera: public ICamera {
public:
   Camera(vec3 start);
   ~Camera();
   void rotLocal(float pitch, float yaw);
   void moveInOut(float dist);
private:
   float m_pitch, m_yaw, m_dist;
};

#endif
