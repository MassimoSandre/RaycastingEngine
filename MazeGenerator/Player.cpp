#include "Player.h"
#include "utils/utils.cpp"
#include "entity/entities/MovingEntity.h"
#include <cmath>

Player::Player(double fov, int nRays, double raysLength)  {
	this->fov = fov;
	this->nRays = nRays;
	this->raysLength = raysLength;
	this->focalLength = ((1.0f / tan(fov / 2.0f)) / 2.0f);

	this->info.resize(nRays);
}

void Player::assignEntityState(EntityState state) {
	this->state = state;

	for (int i = 0; i < nRays; i++) {
		Ray r(state.position, raysLength, state.angle);
		rays.push_back(std::make_shared<Segment>(r));
	}
	this->pointRaysToView();
}


void Player::pointRaysToView() {
	double x = 0;
	double angle;

	for (int i = 0; i < this->nRays; i++) {
		x = (double(i) / double(this->nRays)) - 0.5;
		angle = atan2(x, this->focalLength);
		this->rays[i]->setAngle(this->state.angle + angle);
	}
}
void Player::castWall(ObstacleState& wall, int wallFace) {
	for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection = this->rays[i]->getIntersection(wall.segments[wallFace]);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		if (intersection.intersection.x == 0 && intersection.intersection.y == 0) continue;

		double d1 = intersection.intersection.distance(this->state.position);
		if (d1 == 0) break;

		//if (d1 > this->raysLength) continue;
		if (d1 > this->rays[i]->length) continue;

		double d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;
		
		this->info[this->nRays - 1 - i].push_back({  d1 * (double)cos(this->rays[i]->angle - this->state.angle), intersection.colOffset, wall.height, wall.verticalOffset, wall.owner });

		this->rays[i]->changeP2(intersection.intersection);
	}
}

//void Player::castEntity(std::shared_ptr<Entity> segment, int entityId) {}

void Player::pointTo(Coordinates p) {
	this->state.angle = this->state.position.getAngle(p);
	this->pointRaysToView();
}

void Player::rotate(double angle) {
	this->state.angle += angle;
		
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(this->rays[i]->angle + angle);
	}
}

void Player::update() {
	if (this->nRays > 0) {
		if (this->state.position.x == this->rays[0]->p1.x && this->state.position.y == this->rays[0]->p1.y) return;
	}
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->move(this->state.position);
	}
}
Segment Player::moveForward(double distance) {
	double angle = this->state.angle;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}
Segment Player::moveBackward(double distance) {
	double angle = this->state.angle + 3.1415;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}
Segment Player::moveLeftward(double distance) {
	double angle = this->state.angle + 3.1415 / 2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}
Segment Player::moveRightward(double distance) {
	double angle = this->state.angle - 3.1415 / 2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}


void Player::cast(std::vector<std::shared_ptr<Segment>> segments)/*, std::vector<std::shared_ptr<Entity>> entities); */ {

}

void Player::betterCast(std::vector<ObstacleState>& states)/*, std::vector<std::shared_ptr<Entity>>& entities); */ {
	for (int i = 0; i < this->nRays; i++) {
		this->info[i].clear();
		this->rays[i]->setLength(this->raysLength);
	}

	for (ObstacleState& e : states) {	
		for (int i = 0; i < e.segments.size(); i++) {
			if (e.segments[i].length <= this->raysLength &&
				e.segments[i].p1.distance(this->state.position) >= 2 * this->raysLength &&
				e.segments[i].p2.distance(this->state.position) >= 2 * this->raysLength) continue;

			this->castWall(e, i);
		}
	}

	/*for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->length <= this->raysLength &&
			entities[i]->p1.distance(this->center) >= 2 * this->raysLength &&
			entities[i]->p2.distance(this->center) >= 2 * this->raysLength) continue;

		this->castEntity(entities[i], i);
	}*/
}

ViewInfo Player::getFixedDistances() {
	return this->info;
}