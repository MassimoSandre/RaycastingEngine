#pragma once
#include "../ObstacleProperties.h"
#include "../../Texture.h"

class ObstacleState;
class IObstacle {
protected:
	double HEIGHT;
	double VERTICAL_OFFSET;

	std::shared_ptr<IObstacle> self;

public:
	Texture texture;

	IObstacle() {}
	IObstacle(ObstacleProperties* properties, std::string textureFileName) : texture(textureFileName) {
		this->HEIGHT = properties->height;
		this->VERTICAL_OFFSET = properties->verticalOffset;
	}

	virtual ObstacleState getDefaultState() = 0;

};