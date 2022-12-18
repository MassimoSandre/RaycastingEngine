#pragma once
#include "utils/Segment.h"
#include "entity/entities/IEntity.h"
#include "entity/EntityState.h"

class Collectible : public IEntity {
public:
	Collectible();
	Collectible(EntityProperties* properties, std::string textureFileName);

	EntityState getDefaultState() override;
};