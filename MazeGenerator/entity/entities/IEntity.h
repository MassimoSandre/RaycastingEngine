#pragma once

#include <memory>
#include "../../Texture.h"
#include "../EntityProperties.h"

class EntityState;
class IEntity {
	double HEIGHT;
	double VERTICAL_OFFSET;

	std::shared_ptr<IEntity> self;

public:
	Texture texture;

	IEntity() {}
	IEntity(EntityProperties* properties, std::string textureFileName) : texture(textureFileName) {
		this->HEIGHT = properties->height;
		this->VERTICAL_OFFSET = properties->verticalOffset;
	}

	virtual EntityState getDefaultState() = 0;
};

