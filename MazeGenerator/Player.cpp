#include "Player.h"
#include "Ray.h"
#include "utils.cpp"
#include <cmath>


Player::Player(Coordinates center, double fov, int nRays, double raysLength, double baseAngle) : SmartEntity(center, 0, fov, nRays, raysLength, baseAngle) {}