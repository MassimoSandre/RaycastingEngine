#include "Entity.h"

Entity::Entity() {
	this->type = EntitySegment;
}

Entity::Entity(Coordinates center, double length, double angle) : Segment() {
	this->center = center;
	this->type = EntitySegment;
	double distanceFromCenter = length / 2;

	Coordinates newP1 = center, newP2 = center;

	newP1.x += distanceFromCenter * cos(angle + 3.1415f / 2.0f);
	newP1.y -= distanceFromCenter * sin(angle + 3.1415f / 2.0f);

	newP2.x += distanceFromCenter * cos(angle - 3.1415f / 2.0f);
	newP2.y -= distanceFromCenter * sin(angle - 3.1415f / 2.0f);

	this->changePoints(newP1, newP2);
}

void Entity::faceTo(Coordinates p) {
	double angle = center.getAngle(p);

	Coordinates newP1 = this->center, newP2 = this->center;

	newP1.x = int(center.x + (this->length/2) * cos(angle + 3.1415f / 2.0f));
	newP1.y = int(center.y - (this->length / 2) * sin(angle + 3.1415f / 2.0f));

	newP2.x = int(center.x + (this->length / 2) * cos(angle - 3.1415f / 2.0f));
	newP2.y = int(center.y - (this->length / 2) * sin(angle - 3.1415f / 2.0f));


	this->p1 = newP1;
	this->p2 = newP2;
	this->angle = angle - 3.1415f / 2.0f;
}

bool Entity::collideWithEntity(std::shared_ptr<Entity> entity) {
	return entity->center.distance(this->center) <= entity->length;
}