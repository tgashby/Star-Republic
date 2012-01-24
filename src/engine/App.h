#pragma once

#include "ProjIncludes.h"

class App
{
public:
	App();
	virtual ~App();

	virtual void init() = 0;
	virtual void run() = 0;
	virtual void shutDown() = 0;
};

