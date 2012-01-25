
#include "Coordinate.h"

Coordinate::Coordinate(float x, float y, float z) 
{
  this->x = x;
  this->y = y;
  this->z = z;
}

float Coordinate::getX() 
{
  return x;
}

float Coordinate::getY()
{
  return y;
}

float Coordinate::getZ()
{
  return z;
}
