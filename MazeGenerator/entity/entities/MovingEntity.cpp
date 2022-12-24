#include "MovingEntity.h"

MovingEntity::MovingEntity() : IEntity() {}
MovingEntity::MovingEntity(IElementProperties* properties, std::string textureFileName) : IEntity(properties, textureFileName) {
	this->self = std::make_shared<MovingEntity>(*this);
}

EntityState MovingEntity::getDefaultState() {
	EntityState state;
	state.height = this->HEIGHT;
	state.verticalOffset = this->VERTICAL_OFFSET;
	state.owner = this->self;

	return state;
}

Segment MovingEntity::move(EntityState& state, Coordinates offsets) {
	Segment s(state.position, state.position.add(offsets));
	return s;
}

void MovingEntity::tryMove(EntityState& state, Segment& move, std::vector<ObstacleState>& walls, double multiplier) {
	double moveLen = move.length;
	move.setLength(MOVE_CHECK_DISTANCE);

	Segment moveX(move.p1, { move.p2.x, move.p1.y });
	Segment moveY(move.p1, { move.p1.x, move.p2.y });

	double moveXCoef = moveX.length / move.length;
	double moveYCoef = moveY.length / move.length;

	bool possible = true;
	for (int i = 0; i < walls.size(); i++) {
		for (int j = 0; j < walls[i].segments.size(); j++) {
			IntersectionInfo info;
			moveX.getIntersection(&info, &walls[i].segments[j]);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				double d1 = info.intersection.distance(moveX.p1);

				double d2 = info.intersection.distance(moveX.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;

				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}

		}
	}
	if (possible) {
		moveX.setLength(moveLen * moveXCoef * multiplier);
		applyMove(state, moveX);
		moveY.translate({ double(moveX.p1.x > moveX.p2.x ? -moveX.length : +moveX.length) , 0 });
	}

	possible = true;
	for (int i = 0; i < walls.size(); i++) {
		for (int j = 0; j < walls[i].segments.size(); j++) {
			IntersectionInfo info;
			moveY.getIntersection(&info, &walls[i].segments[j]);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				double d1 = info.intersection.distance(moveY.p1);

				double d2 = info.intersection.distance(moveY.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;

				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}
		}
	}
	if (possible) {
		moveY.setLength(moveLen * moveYCoef * multiplier);
		applyMove(state, moveY);
	}
}

void MovingEntity::applyMove(EntityState& state, Segment& moveSegment) {
	state.position = moveSegment.p2;
}