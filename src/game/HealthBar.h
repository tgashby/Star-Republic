//
//  HealthBar.h
//  StarRepub
//
//  Created by Robert Crosby on 3/14/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_HealthBar_h
#define StarRepub_HealthBar_h

#define HEALTH_BAR_TEXTURE1 "textures/HealthBar.bmp"
#define HEALTH_BAR_TEXTURE2 "textures/HealthBarStable.bmp"
#define HEALTH_BAR_TEXTURE3 "textures/HealthSection.bmp"

#include "Interfaces.h"
#include "Object2d.h"
#include "Mesh.h"

class HealthBar: public Object2d {
public:
   HealthBar(Modules *modules);
   ~HealthBar();
   void updateBar(float healthPercentage);
private:
   Mesh *m_mainMesh;
};

#endif
