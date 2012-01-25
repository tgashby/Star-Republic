#include "World.h"

World::World()
{
	lines.push_back(new Line(0, 0, 0, 0, 0, 10, 0, 0, 0, 0));
}

World::~World()
{
}
Line* World::getInitialLine()
{
	return lines.front;
}
