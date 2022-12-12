#include "SmartEntity.h"

void SmartEntity::pointRaysToView() {
	double x = 0;
	double angle;

	for (int i = 0; i < this->nRays; i++) {
		x = (double(i) / double(this->nRays)) - 0.5;
		angle = atan2(x, this->focalLength);
		this->rays[i]->setAngle(this->baseAngle + angle);
	}
}
void SmartEntity::castWall(std::shared_ptr<Segment> segment, int wallId) {
	for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection = this->rays[i]->getIntersection(segment);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		if (intersection.intersection.x == 0 && intersection.intersection.y == 0) continue;

		double d1 = intersection.intersection.distance(this->center.toInt());
		if (d1 == 0) break;

		//if (d1 > this->raysLength) continue;
		if (d1 > this->rays[i]->length) continue;

		double d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;

		//this->colOffsets[i] = intersection.colOffset;
		if (this->info[this->nRays - 1 - i].size() == 0)
			this->info[this->nRays - 1 - i].push_back({ Obstacle, d1 * (double)cos(this->rays[i]->angle - this->baseAngle), intersection.colOffset, wallId, segment->textureId, segment->height });
		else
			this->info[this->nRays - 1 - i][0] = { Obstacle, d1 * (double)cos(this->rays[i]->angle - this->baseAngle), intersection.colOffset, wallId, segment->textureId, segment->height };

		this->rays[i]->changeP2(intersection.intersection);
	}
}
void SmartEntity::castEntity(std::shared_ptr<Entity> segment, int entityId) {
	for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection = this->rays[i]->getIntersection(segment);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		double d1 = intersection.intersection.distance(this->center);
		if (d1 == 0) break;

		//if (d1 > this->raysLength) continue;
		if (d1 > this->rays[i]->length) continue;

		double d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;

		this->info[this->nRays - 1 - i].push_back({ EntitySegment, d1 * (double)cos(this->rays[i]->angle - this->baseAngle), intersection.colOffset , entityId, segment->textureId, segment->height });
	}
}

SmartEntity::SmartEntity() : MovingEntity() {}
SmartEntity::SmartEntity(Coordinates center, double segmentLength, double fov, int nRays, double raysLength, double baseAngle) : MovingEntity(center, segmentLength, 0.0) {
	this->center = center;
	this->fov = fov;
	this->nRays = nRays;
	this->raysLength = raysLength;
	this->baseAngle = baseAngle;
	this->focalLength = ((1.0f / tan(fov / 2.0f)) / 2.0f);

	this->info.resize(nRays);

	for (int i = 0; i < nRays; i++) {
		Ray r(center, raysLength, baseAngle);
		rays.push_back(std::make_shared<Segment>(r));
	}
	this->pointRaysToView();
}

void SmartEntity::pointTo(Coordinates p) {
	this->baseAngle = this->center.getAngle(p);
	this->pointRaysToView();
}

void SmartEntity::rotate(double angle) {
	this->baseAngle += angle;
	
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(this->rays[i]->angle + angle);
	}
}

void SmartEntity::update()  {
	if (this->nRays > 0) {
		if (this->center.x == this->rays[0]->p1.x && this->center.y == this->rays[0]->p1.y) return;
	}
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->move(this->center);
	}
}

Segment SmartEntity::moveForward(double distance) {
	double angle = this->baseAngle;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}
Segment SmartEntity::moveBackward(double distance) {
	double angle = this->baseAngle + 3.1415;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}
Segment SmartEntity::moveLeftward(double distance) {
	double angle = this->baseAngle + 3.1415 / 2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}
Segment SmartEntity::moveRightward(double distance) {
	double angle = this->baseAngle - 3.1415 / 2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return this->move(o);
}

void SmartEntity::cast(std::vector<std::shared_ptr<Segment>> segments, std::vector<std::shared_ptr<Entity>> entities) {
	for (int i = 0; i < this->nRays; i++) {
		this->info[i].clear();
		this->rays[i]->setLength(this->raysLength);
	}
	for (int i = 0; i < segments.size(); i++) {
		if (segments[i]->length <= this->raysLength &&
			segments[i]->p1.distance(this->center) >= 2 * this->raysLength &&
			segments[i]->p2.distance(this->center) >= 2 * this->raysLength) continue;

		this->castWall(segments[i], i);
	}
	this->pointRaysToView();

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->length <= this->raysLength &&
			entities[i]->p1.distance(this->center) >= 2 * this->raysLength &&
			entities[i]->p2.distance(this->center) >= 2 * this->raysLength) continue;

		this->castEntity(entities[i], i);
	}
}

RenderingInfo SmartEntity::getFixedDistances() {
	return this->info;
}