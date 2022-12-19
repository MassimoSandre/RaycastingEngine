#pragma once
#include "obstacles/Wall.h"
#include "ObstacleProperties.h"

namespace Obstacles {

	static Wall darkWall;
	static Wall brickWall;

	static void load() {
		Obstacles::darkWall = Wall(ObstacleProperties::getDefault()
			->setHeight(16000)
			->setVerticalOffset(0),
			"textures/wallAlt.texture");

		Obstacles::brickWall = Wall(ObstacleProperties::getDefault()
			->setHeight(16000)
			->setVerticalOffset(0),
			"textures/brickwall.texture");
	}
};