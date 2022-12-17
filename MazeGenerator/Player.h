#pragma once

#include "SmartEntity.h"
#include "utils/utils.cpp"

class Player : public SmartEntity {
public:
	Player(Coordinates center, double fov, int nRays, double raysLength, double baseAngle);
};