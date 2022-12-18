#pragma once
#include "../../utils/Segment.h"
#include "IEntity.h"
#include "../EntityState.h"

class Collectible : public IEntity {
public:
	Collectible();
	Collectible(EntityProperties* properties, std::string textureFileName);

	EntityState getDefaultState() override;
};