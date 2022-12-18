#pragma once
#include <memory>
#include <vector>
#include "entities/IEntity.h"
#include "../Segment.h"

class IEntity;
class EntityState {
public:
	std::shared_ptr <IEntity> owner;
	std::vector<Segment> segments;
	double height;
	double verticalOffset;


	EntityState();

	template <typename... Points>
	void setPoints(std::initializer_list<Coordinates> l);
	void setPoints(Rect rect);

	template <typename... Points>
	EntityState withPoints(std::initializer_list<Coordinates> l);
	EntityState withPoints(Rect rect);

};