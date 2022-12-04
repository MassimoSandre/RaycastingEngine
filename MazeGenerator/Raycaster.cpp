#include "Raycaster.h"
#include "Ray.h"
#include "utils.cpp"
#include <cmath>

Raycaster::Raycaster(Coordinates center, double fov, int nRays, float raysLength, double baseAngle) {
	this->center = center;
	this->fov = fov;
	this->nRays = nRays;
	this->raysLength = raysLength;
	this->baseAngle = baseAngle;


	baseAngle -= fov / 2;
	double angleDelta = (float)fov / float(nRays - 1);

	for (int i = 0; i < nRays; i++) {
		Ray r(center, raysLength, baseAngle);
		rays.push_back(std::make_shared<Segment>(r));
		baseAngle += angleDelta;
	}
}

void Raycaster::pointTo(Coordinates p) {
	this->baseAngle = this->center.getAngle(p);

	double angleDelta = (float)this->fov / (this->nRays - 1);

	double startingAngle = this->baseAngle - fov / 2;

	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(startingAngle);
		startingAngle += angleDelta;
	}
}

void Raycaster::rotate(double angle) {
	this->baseAngle += angle;
}

void Raycaster::cast(std::shared_ptr<Segment> segment) {
	for (int i = 0; i < this->nRays; i++) {
		Coordinates intersection = this->rays[i]->getIntersection(segment);

		float d1 = intersection.distance(this->center.toInt());
		if (d1 == 0) break;
		//if (d1 > this->raysLength) continue;
		if (d1 > this->rays[i]->length) continue;

		float d2 = intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;

		this->rays[i]->changeP2(intersection);
	}
}
void Raycaster::cast(std::vector<std::shared_ptr<Segment>> segments) {
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setLength(this->raysLength);
	}
	for (int i = 0; i < segments.size(); i++) {
		if (segments[i]->length <= this->raysLength&&
			segments[i]->p1.distance(this->center) >= 2*this->raysLength &&
			segments[i]->p2.distance(this->center) >= 2*this->raysLength) continue;

		this->cast(segments[i]);
	}

	double angleDelta = (float)this->fov / (this->nRays - 1);

	double startingAngle = this->baseAngle - fov / 2;

	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(startingAngle);
		startingAngle += angleDelta;
	}
}

void Raycaster::move(Coordinates offsets) {
	this->center.x += offsets.x;
	this->center.y += offsets.y;
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->translate(offsets);
	}
}
void Raycaster::moveTo(Coordinates newPosition) {
	Coordinates o = { this->center.x - newPosition.x, this->center.y - newPosition.y };
	this->move(o);
}

void Raycaster::follow(Coordinates target, float distance) {
	if (target.distance(center) <= 2) return;
	double angle = this->center.getAngle(target);
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	this->move(o);
}

void Raycaster::moveForward(float distance) {
	double angle = this->baseAngle;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	this->move(o);
}
void Raycaster::moveBackward(float distance) {
	double angle = this->baseAngle + 3.1415;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	this->move(o);
}
void Raycaster::moveLeftward(float distance) {
	double angle = this->baseAngle + 3.1415/2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	this->move(o);
}
void Raycaster::moveRightward(float distance) {
	double angle = this->baseAngle - 3.1415/2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	this->move(o);
}

std::vector<Coordinates> Raycaster::getFixedDistances() {
	std::vector<Coordinates> d;

	double startingAngle = this->fov / 2;
	double angleDelta = (float)fov / (nRays - 1);

	for (int i = this->nRays - 1; i >= 0; i--) {
		d.push_back(Coordinates{ this->rays[i]->length * (float)cos(startingAngle) , this->raysLength * (float)cos(startingAngle) });

		startingAngle -= angleDelta;
	}

	return d;
}