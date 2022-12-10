#pragma once
#include "utils.cpp"
#include <iostream>
#include <vector>
#include "Segment.h"

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

	std::vector<std::shared_ptr<Segment>> getWalls(Size cellSize, Coordinates offset, double wallThinkness=1.0f);

};