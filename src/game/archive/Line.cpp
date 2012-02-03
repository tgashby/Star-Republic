#include "Line.h"


/**
 * @param sx Starting x coordinate 
 * @param sy Starting y coordinate 
 * @param sz Starting z coordinate 
 * @param ex Ending x coordinate 
 * @param ey Ending y coordinate 
 * @param ez Ending z coordinate 
 * @param ez Ending z coordinate 
 */
Line::Line(float sx, float sy, float sz, float ex, float ey, float ez, float startPitch, float startYaw, float endPitch, float endYaw)
{
	start = new Vector3<float>(sx, sy, sz);
	end = new Vector3<float>(ex, ey, ez);

	m_startPitch = sp;
	m_startYaw = sy;
	m_endPitch = ep;
	m_endYaw = ey;

	float distance = sqrt((sx - ex) * (sx - ex) + 
		(sy - ey) * (sy -ey) + (sz - ez) * (sz - ez));

	while (distance < FRACTALSLICESINLINE)
	{
		list.pushback(new Terrain());
		distance--;
	}	
}
Line::~Line
{

}

Vector3<float> translateToGlobal(float localX, float localY, float localZ)
{
	/** LocalZ MUST be a percentage of the line segment (between 0 and 1) 
	localX and localY are with respect to the center of a terrain slice, i.e. 0,0 is the center of a slice **/
	Vector3<float> returnValue = new Vector3();
	returnValue.x = start.x * localZ + end.x * (1 - localZ) + localX;
	returnValue.y = start.y * localZ + end.y * (1 - localZ) + localY;
	returnValue.z = start.z * localZ + end.z * (1 - localZ); 
	
	return returnValue;
}
Vector3<float> globalRotation(float localZ)
{
	/** LocalZ MUST be a percentage of the line segment (between 0 and 1) **/
	Vector3<float> returnValue = new Vector3();
	returnValue.x = StartPitch * localZ + EndPitch * (1 - localZ);
	returnValue.y = StartYaw * localZ + EndYaw * (1 - localZ);
	returnValue.z = 0; 	

	return returnValue;
}
Vector3<float> getNextLine(float localX, float localY)
{
	/** later... **/
	return this;
}
