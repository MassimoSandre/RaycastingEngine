#pragma once
#include <iostream>
#include <vector>
#include "Segment.h"
#include "Player.h"
#include "Renderer.h"
#include "Entity.h"

class Game {
private:
	Renderer renderer;

	Player player;

	std::vector<std::shared_ptr<Segment>> walls;
	std::vector<std::shared_ptr<Entity>> collectibles;

	bool pause = false;
	bool closing = false;
	
	bool jumping = false;
	float verticalOffset = 0.0f;
	float verticalVelocity = 0.0f;

	Size screenSize;

	Segment move;


	void tryMove(Segment move, float multiplier);
	void keyHandler(float multiplier);
public:
	Game(int nSquare, int windowSquareSize, Size mazeAreaSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle, double fov, int noRays, float viewLength);
	~Game();

	bool update(float elapsedTime);
	void render();

	void addWall(std::shared_ptr<Segment> segment);
	void addWalls(std::vector<std::shared_ptr<Segment>> segments);

	void addCollectible(std::shared_ptr<Entity> collectible);
	void addCollectibles(std::vector<std::shared_ptr<Entity>> collectibles);
};