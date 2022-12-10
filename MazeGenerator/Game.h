#pragma once
#include <iostream>
#include <vector>
#include "Segment.h"
#include "Player.h"
#include "Renderer.h"
#include "Entity.h"
#include "Maze.h"


#define MOVE_DISTANCE 1 

#define MAZE_CANVAS 1
#define PROJECTION_CANVAS 2

#define MINIMAP_SIZE 200
#define MINIMAP_RANGE 200

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

	double verticalOffset = 0.0f;
	double verticalVelocity = 0.0f;

	int mazeSizeIncrement; 
	double wallThickness;

	Size screenSize;
	Size currentMazeSize;
	Size cellSize;

	Segment move;

	Canvas mazeDrawingCanvas, projectionDrawingCanvas;

	void keyHandler(double multiplier);
	bool levelCompleted();
	void newLevel();
	void placeCollectible(std::shared_ptr<Entity>& e);
	void renderMinimap();
public:
	Game(Size windowSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle, double fov, int noRays, double viewLength, Size firstMazeSize, int mazeSizeIncrement, Size cellSize, double wallThickness);
	~Game();

	bool update(double elapsedTime);
	void render();

	void addWall(std::shared_ptr<Segment> segment);
	void addWalls(std::vector<std::shared_ptr<Segment>> segments);

	void addCollectible(std::shared_ptr<Entity> collectible);
	void addCollectibles(std::vector<std::shared_ptr<Entity>> collectibles);
};