#pragma once
#include "Segment.h"
class Ray : public Segment {
public:
	Ray();
	Ray(Coordinates p1, Coordinates p2);
	Ray(Coordinates p1, double length, double angle);
};