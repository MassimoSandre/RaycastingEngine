#pragma once
#include "utils/utils.cpp"
#include "obstacle/ObstacleState.h"
#include "utils/ViewInfo.h"
#include "entity/entities/IEntity.h"

class Player  {
private:
	EntityState state;

	ViewInfo info;
	
	double fov;
	int nRays;
	double raysLength;
	double baseAngle;
	double focalLength;
	
	void pointRaysToView();
	void castWall(ObstacleState& wall, int wallFace);
	//void castEntity(std::shared_ptr<Entity> segment, int entityId);

public:
	Player(Coordinates center, double fov, int nRays, double raysLength, double baseAngle);

	std::vector<std::shared_ptr<Segment>> rays;
	
	void pointTo(Coordinates p);
	
	void rotate(double angle);
	
	void update();
	Segment moveForward(double distance);
	Segment moveBackward(double distance);
	Segment moveLeftward(double distance);
	Segment moveRightward(double distance);
	
	
	void cast(std::vector<std::shared_ptr<Segment>> segments);//, std::vector<std::shared_ptr<Entity>> entities);
	void betterCast(std::vector<ObstacleState>& states);// , std::vector<std::shared_ptr<Entity>>& entities);
	
	ViewInfo getFixedDistances();
};