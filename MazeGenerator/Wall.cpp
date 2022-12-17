#include "Wall.h"
Wall::Wall() : MapElement() {
	this->self = std::make_shared<Wall>(*this);
}
Wall::Wall(ElementProperties* properties, std::string textureFileName) : MapElement(properties, textureFileName) {
	this->self = std::make_shared<Wall>(*this);
}

ElementState Wall::getDefaultState() {
	ElementState state;
	state.height = this->HEIGHT;
	state.verticalOffset = this->VERTICAL_OFFSET;
	state.owner = this->self;

	return state;
}