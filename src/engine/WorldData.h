//
//  WorldData.h
//  StarRepub
//
//  Created by Robert Crosby on 2/18/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_WorldData_h
#define StarRepub_WorldData_h

#include <vector>
#include <string>
#include "Vector.h"

using namespace std;

/*
 WorldData contains structs for holding information about the world
 */

enum UNIT_TYPE {
   UNIT_TURRET,
};

struct LocDirData {
   LocDirData(vec3 l, vec3 f, vec3 u) {
      loc = l;
      fwd = f;
      up = u;
   }
   vec3 loc;
   vec3 fwd;
   vec3 up;
};

struct PropData: public LocDirData {
   PropData(string n, vec3 l, vec3 f, vec3 u): LocDirData(l, f, u) {
      name = n;
   }
   string name;
};

struct UnitData: public LocDirData {
   UnitData(UNIT_TYPE t, vec3 l, vec3 f, vec3 u): LocDirData(l, f, u) {
      type = t;
   }
   UNIT_TYPE type;
};

struct PathPointData: public LocDirData {
   PathPointData(ivec4 link, vec3 l, vec3 f, vec3 u): LocDirData(l, f, u) {
      links = link;
   }
   ivec4 links;
   vector<PropData> props;
   vector<UnitData> units;
};

struct WorldData {
   WorldData() {}
   vector<PathPointData> path;
};

#endif
