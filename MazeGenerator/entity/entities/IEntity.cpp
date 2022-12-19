#include "IEntity.h"

IEntity::IEntity() {}
IEntity::IEntity(IElementProperties* properties, std::string textureFileName) : IElement(textureFileName) {
	this->HEIGHT = properties->height;
	this->VERTICAL_OFFSET = properties->verticalOffset;
}

bool IEntity::collideWith(EntityState state, Coordinates p) {
	return state.position.distance(p) <= state.length / 2;
}