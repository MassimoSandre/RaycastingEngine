#include "MovingEntity.h"

MovingEntity::MovingEntity() : Entity() {}
MovingEntity::MovingEntity(Coordinates center, double length, double angle) : Entity(center, length, angle) {}

void MovingEntity::applyMove(Segment& moveSegment) {
	this->center = moveSegment.p2;
}
void MovingEntity::tryMove(Segment& move, std::vector<std::shared_ptr<Segment>>& walls, double multiplier) {
	double moveLen = move.length;
	move.setLength(MOVE_CHECK_DISTANCE);

	Segment moveX(move.p1, { move.p2.x, move.p1.y });
	Segment moveY(move.p1, { move.p1.x, move.p2.y });

	double moveXCoef = moveX.length / move.length;
	double moveYCoef = moveY.length / move.length;

	bool possible = true;
	for (std::shared_ptr<Segment> w : walls) {
		IntersectionInfo info = moveX.getIntersection(*w);

		if (info.intersection.x != -1 || info.intersection.y != -1) {
			double d1 = info.intersection.distance(moveX.p1);

			double d2 = info.intersection.distance(moveX.p2);

			if (d1 > MOVE_CHECK_DISTANCE) continue;


			if (d2 > MOVE_CHECK_DISTANCE) continue;
			possible = false;
			break;
		}

	}
	if (possible) {
		moveX.setLength(moveLen * moveXCoef * multiplier);
		this->applyMove(moveX);
		moveY.translate({ double(moveX.p1.x > moveX.p2.x ? -moveX.length : +moveX.length) , 0 });
	}

	possible = true;
	for (std::shared_ptr<Segment>& w : walls) {
		IntersectionInfo info = moveY.getIntersection(*w);

		if (info.intersection.x != -1 || info.intersection.y != -1) {
			double d1 = info.intersection.distance(moveY.p1);

			double d2 = info.intersection.distance(moveY.p2);

			if (d1 > MOVE_CHECK_DISTANCE) continue;


			if (d2 > MOVE_CHECK_DISTANCE) continue;
			possible = false;
			break;
		}

	}
	if (possible) {
		moveY.setLength(moveLen * moveYCoef * multiplier);
		this->applyMove(moveY);
	}
}

Segment MovingEntity::move(Coordinates offsets) {
	Segment s(this->center, this->center.add(offsets));
	return s;
}
Segment MovingEntity::moveTo(Coordinates newPosition) {
	Coordinates o = { this->center.x - newPosition.x, this->center.y - newPosition.y };
	return this->move(o);
}
Segment MovingEntity::follow(Coordinates target, double distance) {
	if (target.distance(center) <= 2) return Segment(this->center, this->center);
	double angle = this->center.getAngle(target);
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}