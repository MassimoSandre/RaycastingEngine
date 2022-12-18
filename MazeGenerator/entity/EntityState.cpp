#include "EntityState.h"

EntityState::EntityState() {
	this->segments.clear();
	this->height = 0;
	this->verticalOffset = 0;
	//this->owner = nullptr;
}

template <typename... Points>
void EntityState::setPoints(std::initializer_list<Coordinates> l) {
	int i = 0;
	Coordinates p1, p0;
	for (auto& p : l) {
		if (i > 0) {
			this->segments.push_back(Segment(p1, p));
		}
		else p0 = p;

		p1 = p;

		i++;
		if (i == l.size()) {
			this->segments.push_back(Segment(p1, p0));
		}
	}
}

void EntityState::setPoints(Rect rect) {
	Coordinates p1, p2, p3, p4;
	p1.x = rect.topLeft.x;
	p1.y = rect.topLeft.y;

	p2.x = rect.topLeft.x + rect.size.x;
	p2.y = rect.topLeft.y;

	p3.x = rect.topLeft.x + rect.size.x;
	p3.y = rect.topLeft.y + rect.size.y;

	p4.x = rect.topLeft.x;
	p4.y = rect.topLeft.y + rect.size.y;

	this->setPoints({ p1, p2, p3, p4 });
}

template <typename... Points>
EntityState EntityState::withPoints(std::initializer_list<Coordinates> l) {
	this->setPoints(l);
	return *this;
}

EntityState EntityState::withPoints(Rect rect) {
	this->setPoints(rect);
	return *this;
}