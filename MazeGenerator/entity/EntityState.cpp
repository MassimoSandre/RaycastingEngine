#include "EntityState.h"

EntityState::EntityState() {
	this->height = 0;
	this->verticalOffset = 0;
	this->owner = nullptr;
}

void EntityState::faceTo(Coordinates p) {
	double angle = this->position.getAngle(p);
	
	Coordinates newP1 = this->position, newP2 = this->position;

	newP1.x = int(position.x + (this->length / 2) * cos(angle + 3.1415f / 2.0f));
	newP1.y = int(position.y - (this->length / 2) * sin(angle + 3.1415f / 2.0f));

	newP2.x = int(position.x + (this->length / 2) * cos(angle - 3.1415f / 2.0f));
	newP2.y = int(position.y - (this->length / 2) * sin(angle - 3.1415f / 2.0f));


	this->segment.changePoints(newP1, newP2);
}

void EntityState::set(Coordinates position, double length, double angle) {
	this->position = position;
	this->length = length;
	this->angle = angle;
	double distanceFromCenter = length / 2;

	Coordinates newP1 = position, newP2 = position;
	
	newP1.x += distanceFromCenter * cos(angle);
	newP1.y -= distanceFromCenter * sin(angle);

	newP2.x += distanceFromCenter * cos(angle);
	newP2.y -= distanceFromCenter * sin(angle);


	this->segment.changePoints(newP1, newP2);
}

EntityState EntityState::with(Coordinates position, double length, double angle) {
	this->set(position, length, angle);
	return *this;
}