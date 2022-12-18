#pragma once

#include "IEntity.h"
#include "../EntityState.h"
#include "../../obstacle/ObstacleState.h"

#define MOVE_CHECK_DISTANCE 3

class MovingEntity : public IEntity {
public:
	MovingEntity();
	MovingEntity(EntityProperties* properties, std::string textureFileName="");

	EntityState getDefaultState() override;

	static Segment move(EntityState& state, Coordinates offsets);
	
	static void tryMove(EntityState& state, Segment& move, std::vector<ObstacleState>& walls, double multiplier);

	static void applyMove(EntityState& state, Segment& moveSegment);

};

