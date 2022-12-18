#pragma once

#include "IObstacle.h"
#include "../ObstacleProperties.h"
#include "../ObstacleState.h"
#include <string>

class Wall : public IObstacle{
public:
	Wall() ;
	Wall(ObstacleProperties* properties, std::string textureFileName);

	ObstacleState getDefaultState() override;
};