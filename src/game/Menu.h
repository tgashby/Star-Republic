#ifndef StarRepub_Menu_h
#define StarRepub_Menu_h

#include "Interfaces.h"
#include "Camera.h"
#include "Object3d.h"
#include <assert.h>
#include <cmath>

/**
 * Menu object 
 */
class Menu : public Object3d {
public:
   /**
    * Menu construct, takes 2 strings representing the object file path 
    * and the texture file path respectively, a Modules pointer
    * @param fileName the object file, i.e. the model
    * @param textureName the texture file
    * @param modules the Modules pointer
    */
   Menu(string fileName, string textureName, Modules *modules);
   ~Menu();


   Mesh *m_mesh;
   mat4 modelMtx;
private:
};

#endif
