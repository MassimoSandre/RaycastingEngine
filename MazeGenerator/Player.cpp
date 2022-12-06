#include "Player.h"
#include "Ray.h"
#include "utils.cpp"
#include <cmath>

void Player::pointRaysToView() {
	double x = 0;
	double angle;

	for (int i = 0; i < this->nRays; i++) {
		x = (float(i) / float(this->nRays)) - 0.5;
		angle = atan2(x, this->focalLength);
		this->rays[i]->setAngle(this->baseAngle + angle);
	}
}

Player::Player(Coordinates center, double fov, int nRays, float raysLength, double baseAngle) {
	this->center = center;
	this->fov = fov;
	this->nRays = nRays;
	this->raysLength = raysLength;
	this->baseAngle = baseAngle;
	this->colOffsets.resize(nRays);

	this->focalLength = ((1.0f / tan(fov / 2.0f)) / 2.0f);

	this->info.resize(nRays);

	for (int i = 0; i < nRays; i++) {
		Ray r(center, raysLength, baseAngle);
		rays.push_back(std::make_shared<Segment>(r));
	}
	this->pointRaysToView();
}

void Player::pointTo(Coordinates p) {
	this->baseAngle = this->center.getAngle(p);
	this->pointRaysToView();

	/*double angleDelta = (float)this->fov / (this->nRays - 1);

	double startingAngle = this->baseAngle - fov / 2;

	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(startingAngle);
		startingAngle += angleDelta;
	}*/
}

void Player::rotate(double angle) {
	this->baseAngle += angle;
	//this->pointRaysToView();
	return;
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(this->rays[i]->angle + angle);
	}
}

void Player::castWall(std::shared_ptr<Segment> segment) {
	for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection = this->rays[i]->getIntersection(segment);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		if (intersection.intersection.x == 0 && intersection.intersection.y == 0) continue;

		float d1 = intersection.intersection.distance(this->center.toInt());
		if (d1 == 0) break;

		//if (d1 > this->raysLength) continue;
		if (d1 > this->rays[i]->length) continue;

		float d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;

		//this->colOffsets[i] = intersection.colOffset;
		if (this->info[this->nRays - 1 - i].size() == 0)
			this->info[this->nRays - 1 - i].push_back({ Obstacle, d1* (float)cos(this->rays[i]->angle - this->baseAngle), intersection.colOffset });
		else
			this->info[this->nRays - 1 - i][0] = { Obstacle, d1 * (float)cos(this->rays[i]->angle - this->baseAngle), intersection.colOffset };

		this->rays[i]->changeP2(intersection.intersection);
	}
}
void Player::castEntity(std::shared_ptr<Entity> entity) {
	for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection = this->rays[i]->getIntersection(entity);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		float d1 = intersection.intersection.distance(this->center);
		if (d1 == 0) break;

		//if (d1 > this->raysLength) continue;
		if (d1 > this->rays[i]->length) continue;

		float d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;

		this->info[this->nRays - 1 - i].push_back({ EntitySegment, d1*(float)cos(this->rays[i]->angle - this->baseAngle), intersection.colOffset });
	}
}
void Player::cast(std::vector<std::shared_ptr<Segment>> segments, std::vector<std::shared_ptr<Entity>> entities) {
	for (int i = 0; i < this->nRays; i++) {
		this->info[i].clear();
		this->rays[i]->setLength(this->raysLength);
	}
	for (int i = 0; i < segments.size(); i++) {
		if (segments[i]->length <= this->raysLength&&
			segments[i]->p1.distance(this->center) >= 2*this->raysLength &&
			segments[i]->p2.distance(this->center) >= 2*this->raysLength) continue;

		this->castWall(segments[i]);
	}
	this->pointRaysToView();

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->length <= this->raysLength &&
			entities[i]->p1.distance(this->center) >= 2 * this->raysLength &&
			entities[i]->p2.distance(this->center) >= 2 * this->raysLength) continue;

		this->castEntity(entities[i]);
	}
}

void Player::applyMove(Segment& moveSegment) {
	this->center = moveSegment.p2;
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->move(this->center);
	}
}

Segment Player::move(Coordinates offsets) {
	Segment s(this->center, this->center.add(offsets));
	return s;
}
Segment Player::moveTo(Coordinates newPosition) {
	Coordinates o = { this->center.x - newPosition.x, this->center.y - newPosition.y };
	return this->move(o);
}

Segment Player::follow(Coordinates target, float distance) {
	if (target.distance(center) <= 2) return Segment(this->center, this->center);
	double angle = this->center.getAngle(target);
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}

Segment Player::moveForward(float distance) {
	double angle = this->baseAngle;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}
Segment Player::moveBackward(float distance) {
	double angle = this->baseAngle + 3.1415;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}
Segment Player::moveLeftward(float distance) {
	double angle = this->baseAngle + 3.1415/2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}
Segment Player::moveRightward(float distance) {
	double angle = this->baseAngle - 3.1415/2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}

RenderingInfo Player::getFixedDistances() {
	/*for (int i = this->nRays - 1; i >= 0; i--) {
		d.push_back(RenderInfo{ this->rays[i]->length * (float)cos(this->rays[i]->angle - this->baseAngle) , this->raysLength * (float)cos(this->rays[i]->angle - this->baseAngle), this->colOffsets[i]});

	}*/
	return this->info;
}