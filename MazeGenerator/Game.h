#pragma once
#include <iostream>
#include <vector>
#include "Segment.h"
#include "Player.h"
#include "Renderer.h"

class Game {
private:
	Renderer renderer;

	Player player;

	std::vector<std::shared_ptr<Segment>> walls;

	bool pause = false;
	bool closing = false;

	Size screenSize;

	Segment move;


	void tryMove(Segment move);
	void keyHandler();
public:
	Game(Size windowSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle, double fov, double noRays, double viewLength);
	~Game();

	bool update();
	void render();

	void addWall(std::shared_ptr<Segment> segment);
	void addWalls(std::vector<std::shared_ptr<Segment>> segments);
};

