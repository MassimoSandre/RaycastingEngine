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
			->setHeight(12000)
			->setVerticalOffset(0),
			"textures/zombie.texture");
	}
};

