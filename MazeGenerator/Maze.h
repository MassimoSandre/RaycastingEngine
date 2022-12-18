#pragma once
#include <vector>
#include "utils/utils.cpp"
#include "Segment.h"
#include <queue>
#include "obstacle/obstacles/IObstacle.h"
#include "obstacle/ObstacleState.h"


class Maze {
private:
	int width;
	int height;

	std::vector<Line> walls;
public:
	Maze();
	Maze(Size size);


	void generate();
	
	void setSize(Size size);

	std::vector<ObstacleState> getWalls(Size cellSize, Coordinates offset, double wallThinkness, std::shared_ptr<IObstacle> obstacle);

};