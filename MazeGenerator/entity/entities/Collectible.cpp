#include "Collectible.h"

Collectible::Collectible() : IEntity() {}
Collectible::Collectible(EntityProperties* properties, std::string textureFileName) : IEntity(properties, textureFileName) {
	this->self = std::make_shared<Collectible>(*this);
}

EntityState Collectible::getDefaultState() {
	EntityState state;
	state.height = this->HEIGHT;
	state.verticalOffset = this->VERTICAL_OFFSET;
	state.owner = this->self;

	return state;
}