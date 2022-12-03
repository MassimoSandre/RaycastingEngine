#pragma once
#include "Segment.h"

class Wall : public Segment{
public:
	Wall();
	Wall(Coordinates p1, Coordinates p2);
	Wall(Coordinates p1, float length, double angle);
};

