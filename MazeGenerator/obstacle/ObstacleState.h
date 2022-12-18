#pragma once

#include <vector>
#include <memory>
#include "../utils/Segment.h"
#include "../utils/utils.cpp"
#include "../IElement.h"

class IObstacle; 
class ObstacleState {	
public:
	std::shared_ptr <IObstacle> owner;
	std::vector<Segment> segments;
	double height;
	double verticalOffset;


	ObstacleState();

	void setPoints(std::initializer_list<Coordinates> l);
	void setPoints(Rect rect);

	ObstacleState withPoints(std::initializer_list<Coordinates> l);
	ObstacleState withPoints(Rect rect);
};