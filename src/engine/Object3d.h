#ifndef SimpleGame_Object3d_h
#define SimpleGame_Object3d_h

#include "Interfaces.h"

class Object3d : public IObject3d {
public:
   Object3d();
   Object3d(string fileName, string textureName);
   ~Object3d();
   list<IMesh *>* getMeshes();
protected:
   list<IMesh *> m_meshList;
};

#endif
