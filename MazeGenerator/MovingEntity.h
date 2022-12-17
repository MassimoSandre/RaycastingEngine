#pragma once

#include "Entity.h"

#define MOVE_CHECK_DISTANCE 3

class MovingEntity : public Entity {
public:
	MovingEntity();
	MovingEntity(Coordinates center, double length, double angle);

	void applyMove(Segment& moveSegment);
	void tryMove(Segment& move, std::vector<std::shared_ptr<Segment>>& walls, double multiplier);

	Segment move(Coordinates offsets);
	Segment moveTo(Coordinates newPosition);
	Segment follow(Coordinates target, double distance);
};

