#pragma once
#include <memory>
#include <vector>
#include "entities/IEntity.h"
#include "../Segment.h"

class IEntity;
class EntityState {
public:
	std::shared_ptr <IEntity> owner;
	Segment segment;
	Coordinates position;
	double length;
	double angle;

	double height;
	double verticalOffset;


	EntityState();

	void faceTo(Coordinates p);

	void set(Coordinates position, double length, double angle=0);
	EntityState with(Coordinates position, double length, double angle=0);

};