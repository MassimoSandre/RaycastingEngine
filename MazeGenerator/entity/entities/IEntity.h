#pragma once

#include <memory>
#include "../../utils/Texture.h"
#include "../EntityProperties.h"
#include "../EntityState.h"

class EntityState;
class IEntity {
protected:
	double HEIGHT;
	double VERTICAL_OFFSET;

	std::shared_ptr<IEntity> self;

public:
	Texture texture;

	IEntity();
	IEntity(EntityProperties* properties, std::string textureFileName);

	static bool collideWith(EntityState state, Coordinates p);

	virtual EntityState getDefaultState() = 0;
};

