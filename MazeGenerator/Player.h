#pragma once
#include "utils.cpp"
#include <iostream>
#include <vector>
#include "Ray.h"
#include "Segment.h"

class Player {
private:
	Coordinates center;
	double fov;
	int nRays;
	float raysLength;
	double baseAngle;
	float focalLength;
public:

	std::vector<std::shared_ptr<Segment>> rays;
	std::vector<float> colOffsets;

	Player(Coordinates center, double fov, int nRays, float raysLength, double baseAngle);

	void pointTo(Coordinates p);

	void rotate(double angle);

	void cast(std::shared_ptr<Segment> segment);
	void cast(std::vector<std::shared_ptr<Segment>> segments);

	void applyMove(Segment& moveSegment);

	Segment move(Coordinates offsets);
	Segment moveTo(Coordinates newPosition);
	Segment follow(Coordinates target, float distance);

	Segment moveForward(float distance);
	Segment moveBackward(float distance);
	Segment moveLeftward(float distance);
	Segment moveRightward(float distance);

	std::vector<RenderInfo> getFixedDistances();

};

