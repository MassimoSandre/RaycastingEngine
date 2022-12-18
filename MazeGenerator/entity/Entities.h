#pragma once

#include "entities/MovingEntity.h"
#include "entities/Collectible.h"
#include "EntityProperties.h"

namespace Entities {
	static MovingEntity player;

	static Collectible coin;

	static void load() {
		Entities::player = MovingEntity(EntityProperties::getDefault());

		Entities::coin = Collectible(EntityProperties::getDefault()
			->setHeight(4000)
			->setVerticalOffset(1000),
			"textures/collectible.texture");
	}
};

