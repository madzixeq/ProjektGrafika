#pragma once
#include "ModelContext.h"

class Cube
{
	ModelContext* model;

public:
	bool init();
	void draw();
};
