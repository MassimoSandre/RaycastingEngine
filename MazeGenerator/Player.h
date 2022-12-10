#pragma once
#include "utils.cpp"
#include <iostream>
#include <vector>
#include "Ray.h"
#include "Segment.h"
#include "Entity.h"	
#include "SmartEntity.h"

class Player : public SmartEntity {
public:
	Player(Coordinates center, double fov, int nRays, double raysLength, double baseAngle);
};