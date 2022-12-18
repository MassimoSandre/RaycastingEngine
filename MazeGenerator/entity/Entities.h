#pragma once

#include "../MovingEntity.h"
#include "EntityProperties.h"

namespace Entities {
	static MovingEntity player;

	static void load() {
		Entities::player = MovingEntity(EntityProperties::getDefault());
	}
};

