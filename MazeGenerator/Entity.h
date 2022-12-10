#pragma once
#include "Segment.h"
class Entity : public Segment {
public:
	Coordinates center;

	Entity();
	Entity(Coordinates center, double length, double angle);

	void faceTo(Coordinates p);

	bool collideWithEntity(std::shared_ptr<Entity> entity);
};