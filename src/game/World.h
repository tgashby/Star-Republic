#ifndef LIST
#define LIST
#include <list>
#endif

#include "Line.h"

class World
{
public:
	std::list<Line> lines;
	

	World();
	~World();
	Line* getInitialLine();
		

}
