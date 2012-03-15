#ifndef StarRepub_Object3d_h
#define StarRepub_Object3d_h

#include "Interfaces.h"

class Object3d : public IObject3d {
public:
   Object3d(string fileName, string textureName, Modules *modules);
   Object3d();
   virtual ~Object3d();
   list<IMesh *>* getMeshes();
   void setVisible(bool mood);
   virtual bool viewCull(vector<vec4> *planes);
protected:
   list<IMesh *> m_meshList;
};

#endif
