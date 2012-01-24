#ifndef MATH
#define MATH
#include <math.h>
#endif

#ifndef LIST
#define LIST
#include <list>
#endif

class Line
{
public:
	static const float FRACTALSLICESINLINE 1.0f;

	Line* nextLines;
	std::list<Terrain> slices;
	GameObjects* enemies;
	/** Global Coordinates **/
	Vector3<float> start, end;
	float StartPitch, StartYaw, EndPitch, EndYaw;

	Line(float sx, float sy, float sz, float ex, float ey, float ez, float sp, float sy, float es, float ey);
	~Line;

	Vector3<float> translateToGlobal(float localX, float localY, float localZ);
	Vector3<float> globalRotation(float localZ);
	Vector3<float> getNextLine(float localX, float localY);
};
