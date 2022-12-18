#pragma once

#include "IEntity.h"
#include "../EntityState.h"

#define MOVE_CHECK_DISTANCE 3

class MovingEntity : public IEntity {
public:
	/*MovingEntity();
	MovingEntity(Coordinates center, double length, double angle);

	void applyMove(Segment& moveSegment);
	void tryMove(Segment& move, std::vector<std::shared_ptr<Segment>>& walls, double multiplier);

	Segment move(Coordinates offsets);
	Segment moveTo(Coordinates newPosition);
	Segment follow(Coordinates target, double distance);*/

	MovingEntity();
	MovingEntity(EntityProperties* properties, std::string textureFileName="");

	EntityState getDefaultState() override;

	static Segment move(EntityState& state, Coordinates offsets);
	
	static void tryMove(EntityState& state, Segment& move, std::vector<std::shared_ptr<Segment>>& walls, double multiplier);

	static void applyMove(EntityState& state, Segment& moveSegment);

};

