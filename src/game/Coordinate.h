
#pragma once

/* 
 * Coordinate - Says where something is or where it's going 
 */

class Coordinate {
 public:
  Coordinate(float x, float y, float z);
  float getX();
  float getY();
  float getZ();
 private:
  float x;
  float y;
  float z;
};
