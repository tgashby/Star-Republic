#ifndef StarRepub_Object3d_h
#define StarRepub_Object3d_h

#include "Interfaces.h"

class Object3d : public IObject3d {
public:
   Object3d(string fileName, string textureName, Modules *modules);
   ~Object3d();
   list<IMesh *>* getMeshes();
protected:
   list<IMesh *> m_meshList;
};

#endif
