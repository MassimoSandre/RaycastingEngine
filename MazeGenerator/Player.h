#pragma once
#include "utils/utils.cpp"
#include "obstacle/ObstacleState.h"
#include "utils/ViewInfo.h"
#include "entity/entities/IEntity.h"
#include "inventory/Inventory.h"

class Player  {
private:
	ViewInfo info;
	
	double fov;
	int nRays;
	double raysLength;
	double focalLength;
	
	void pointRaysToView();
	void castWall(ObstacleState& wall, int wallFace);
	void castEntity(EntityState& entity);

public:
	Inventory inventory;

	EntityState state;

	Player(double fov, int nRays, double raysLength);

	void assignEntityState(EntityState state);

	std::vector<std::shared_ptr<Segment>> rays;
	
	void pointTo(Coordinates p);
	
	void rotate(double angle);
	
	void update();
	Segment moveForward(double distance);
	Segment moveBackward(double distance);
	Segment moveLeftward(double distance);
	Segment moveRightward(double distance);
	
	void cast(std::vector<ObstacleState>& states, std::vector<EntityState>& entities);
	
	ViewInfo getFixedDistances();
};