#include "Wall.h"
Wall::Wall() : IObstacle() {}
Wall::Wall(IElementProperties* properties, std::string textureFileName) : IObstacle(properties, textureFileName) {
	this->self = std::make_shared<Wall>(*this);
}

ObstacleState Wall::getDefaultState() {
	ObstacleState state;
	state.height = this->HEIGHT;
	state.verticalOffset = this->VERTICAL_OFFSET;
	state.owner = this->self;

	return state;
}