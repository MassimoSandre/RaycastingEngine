#pragma once
#include "utils.cpp"
#include <iostream>
#include <vector>
#include "Ray.h"
#include "Segment.h"
#include "Entity.h"	

class Player {
private:
	double fov;
	int nRays;
	float raysLength;
	double baseAngle;
	float focalLength;

	RenderingInfo info;

	void pointRaysToView();
	void castWall(std::shared_ptr<Segment> segment);
	void castEntity(std::shared_ptr<Entity> segment);
public:
	Coordinates center;

	std::vector<std::shared_ptr<Segment>> rays;
	std::vector<float> colOffsets;

	Player(Coordinates center, double fov, int nRays, float raysLength, double baseAngle);

	void pointTo(Coordinates p);

	void rotate(double angle);

	void cast(std::vector<std::shared_ptr<Segment>> segments, std::vector<std::shared_ptr<Entity>> entities);

	void applyMove(Segment& moveSegment);

	Segment move(Coordinates offsets);
	Segment moveTo(Coordinates newPosition);
	Segment follow(Coordinates target, float distance);

	Segment moveForward(float distance);
	Segment moveBackward(float distance);
	Segment moveLeftward(float distance);
	Segment moveRightward(float distance);

	RenderingInfo getFixedDistances();

};