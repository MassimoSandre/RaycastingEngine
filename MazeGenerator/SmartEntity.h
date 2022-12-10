#pragma once
#include "MovingEntity.h"
#include "Ray.h"

class SmartEntity : public MovingEntity {
protected:
	RenderingInfo info;

	double fov;
	int nRays;
	double raysLength;
	double baseAngle;
	double focalLength;

	void pointRaysToView();
	void castWall(std::shared_ptr<Segment> segment, int wallId);
	void castEntity(std::shared_ptr<Entity> segment, int entityId);

public:
	SmartEntity();
	SmartEntity(Coordinates center, double segmentLength, double fov, int nRays, double raysLength, double baseAngle);

	std::vector<std::shared_ptr<Segment>> rays;

	void pointTo(Coordinates p);

	void rotate(double angle);

	void update();
	Segment moveForward(double distance);
	Segment moveBackward(double distance);
	Segment moveLeftward(double distance);
	Segment moveRightward(double distance);


	void cast(std::vector<std::shared_ptr<Segment>> segments, std::vector<std::shared_ptr<Entity>> entities);

	RenderingInfo getFixedDistances();
};

