#pragma once
#include "utils.cpp"
#include <iostream>
#include <vector>
#include "Ray.h"
#include "Segment.h"

class Raycaster {
private:
	Coordinates center;
	double fov;
	int nRays;
	float raysLength;
	double baseAngle;
public:
	std::vector<std::shared_ptr<Segment>> rays;

	Raycaster(Coordinates center, double fov, int nRays, float raysLength, double baseAngle);

	void pointTo(Coordinates p);

	void rotate(double angle);

	void cast(std::shared_ptr<Segment> segment);
	void cast(std::vector<std::shared_ptr<Segment>> segments);

	void move(Coordinates offsets);
	void moveTo(Coordinates newPosition);
	void follow(Coordinates target, float distance);

	void moveForward(float distance);
	void moveBackward(float distance);
	void moveLeftward(float distance);
	void moveRightward(float distance);

	std::vector<Coordinates> getFixedDistances();
};

