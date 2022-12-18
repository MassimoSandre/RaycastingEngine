#pragma once

#include <memory>
#include "../../utils/Texture.h"
#include "../EntityProperties.h"
#include "../EntityState.h"
#include "../../IElement.h"

class EntityState;
class IEntity : public IElement{
protected:
	std::shared_ptr<IEntity> self;

public:
	IEntity();
	IEntity(EntityProperties* properties, std::string textureFileName);

	static bool collideWith(EntityState state, Coordinates p);

	virtual EntityState getDefaultState() = 0;
};

