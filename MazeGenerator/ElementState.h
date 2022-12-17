#pragma once

#include <vector>
#include <memory>
#include "Segment.h"
#include "utils/utils.cpp"

class MapElement; 
class ElementState {	
public:
	std::shared_ptr <MapElement> owner;
	std::vector<Segment> segments;
	double height;
	double verticalOffset;


	ElementState();

	template <typename... Points>
	void setPoints(std::initializer_list<Coordinates> l);
	void setPoints(Rect rect);

	template <typename... Points>
	ElementState withPoints(std::initializer_list<Coordinates> l);
	ElementState withPoints(Rect rect);
	
};

