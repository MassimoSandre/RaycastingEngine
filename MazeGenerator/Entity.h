#pragma once
#include "Segment.h"
class Entity : public Segment {
public:
	Coordinates center;

	Entity();
	Entity(Coordinates center, float length, double angle);

	void faceTo(Coordinates p);
};