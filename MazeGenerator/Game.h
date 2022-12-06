#pragma once
#include <iostream>
#include <vector>
#include "Segment.h"
#include "Player.h"
#include "Renderer.h"
#include "Entity.h"
#include "Maze.h"

class Game {
private:
	Renderer renderer;
	Maze generator;
	Player player;

	std::vector<std::shared_ptr<Segment>> walls;
	std::vector<std::shared_ptr<Entity>> collectibles;

	bool pause = false;
	bool closing = false;	
	bool jumping = false;

	float verticalOffset = 0.0f;
	float verticalVelocity = 0.0f;

	int mazeSizeIncrement; 
	int wallThickness;

	Size screenSize;
	Size currentMazeSize;
	Size cellSize;


	Segment move;


	void tryMove(Segment move, float multiplier);
	void keyHandler(float multiplier);
	bool levelCompleted();
	void newLevel();
public:
	Game(int nSquare, int windowSquareSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle, double fov, int noRays, float viewLength, Size firstMazeSize, int mazeSizeIncrement, Size cellSize, float wallThickness);
	~Game();

	bool update(float elapsedTime);
	void render();

	void addWall(std::shared_ptr<Segment> segment);
	void addWalls(std::vector<std::shared_ptr<Segment>> segments);

	void addCollectible(std::shared_ptr<Entity> collectible);
	void addCollectibles(std::vector<std::shared_ptr<Entity>> collectibles);
};