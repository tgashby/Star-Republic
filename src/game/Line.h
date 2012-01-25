#ifndef MATH
#define MATH
#include <math.h>
#endif

#ifndef LIST
#define LIST
#include <list>
#endif

#include "Terrain.h"
#include "GameObject.h"

class Line
{
public:
	Line* nextLines;
	std::list<Terrain> slices;
	std::list<GameObject> enemies;
	/** Global Coordinates **/
	Vector3<float> start, end;
	float StartPitch, StartYaw, EndPitch, EndYaw;

	Line(float sx, float sy, float sz, float ex, float ey, float ez, float sp, float sy, float es, float ey);
	~Line;

	Vector3<float> translateToGlobal(float localX, float localY, float localZ);
	Vector3<float> globalRotation(float localZ);
	Vector3<float> getNextLine(float localX, float localY);
};
