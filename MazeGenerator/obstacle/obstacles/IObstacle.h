#pragma once
#include "../ObstacleProperties.h"
#include "../../utils/Texture.h"
#include "../../IElement.h"

class ObstacleState;
class IObstacle : public IElement {
protected:
	std::shared_ptr<IObstacle> self;
public:
	IObstacle() {}
	IObstacle(IElementProperties* properties, std::string textureFileName) : IElement(textureFileName) {
		this->HEIGHT = properties->height;
		this->VERTICAL_OFFSET = properties->verticalOffset;
	}

	virtual ObstacleState getDefaultState() = 0;

};