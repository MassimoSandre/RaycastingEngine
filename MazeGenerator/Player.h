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
	double raysLength;
	double baseAngle;
	double focalLength;

	RenderingInfo info;

	void pointRaysToView();
	void castWall(std::shared_ptr<Segment> segment, int wallId);
	void castEntity(std::shared_ptr<Entity> segment, int entityId);
public:
	Coordinates center;

	std::vector<std::shared_ptr<Segment>> rays;
	std::vector<double> colOffsets;

	Player(Coordinates center, double fov, int nRays, double raysLength, double baseAngle);

	bool collideWithEntity(std::shared_ptr<Entity> entity);

	void pointTo(Coordinates p);

	void rotate(double angle);

	void cast(std::vector<std::shared_ptr<Segment>> segments, std::vector<std::shared_ptr<Entity>> entities);

	void applyMove(Segment& moveSegment);

	Segment move(Coordinates offsets);
	Segment moveTo(Coordinates newPosition);
	Segment follow(Coordinates target, double distance);

	Segment moveForward(double distance);
	Segment moveBackward(double distance);
	Segment moveLeftward(double distance);
	Segment moveRightward(double distance);

	RenderingInfo getFixedDistances();

};