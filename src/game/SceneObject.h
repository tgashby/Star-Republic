#ifndef StarRepub_SceneObject_h
#define StarRepub_SceneObject_h

#include "../engine/Interfaces.h"
#include "Object3d.h"

class SceneObject : public Object3d {
public:
   SceneObject(string fileName, string textureName, vec3 pos, vec3 fwd, vec3 up, Modules *modules);
   ~SceneObject();
};

#endif
