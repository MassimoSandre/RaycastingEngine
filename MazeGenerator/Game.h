#pragma once
#include "Renderer.h"
#include "Player.h"
#include "levelgenerator/Maze.h"
#include <vector>
#include <memory>
#include "utils/Segment.h"
#include "utils/Texture.h"
#include <GLFW/glfw3.h>
#include "obstacle/ObstacleState.h"
#include "obstacle/obstacles/Wall.h"
#include "utils/ViewInfo.h"
#include "obstacle/Obstacles.h"
#include "entity/Entities.h"

#define MOVE_DISTANCE 1 

#define MAZE_CANVAS 1
#define PROJECTION_CANVAS 2

#define MINIMAP_SIZE 150
#define MINIMAP_RANGE 100

#define DEFAULT_WALL_HEIGHT 8000.0
#define PIXEL_HEIGHT_REAL_HEIGHT_RATIO 32.0/DEFAULT_WALL_HEIGHT

#define CHUNK_SIZE 4
#define WALL_PIXEL_HEIGHT 32


class Game {
private:
	Renderer renderer;
	Maze generator;
	Player player;

	//std::vector<std::shared_ptr<Segment>> walls;
	//std::vector<std::shared_ptr<Entity>> collectibles;

	std::vector<std::shared_ptr<Texture>> textures;

	std::vector<ObstacleState> betterWalls;
	std::vector<EntityState> betterCollectibles;

	bool pause = false;
	bool canTogglePause = true;
	bool closing = false;	
	bool jumping = false;

	double cameraVerticalOffset = 0.0;
	double cameraVerticalVelocity = 0.0;

	double currentVerticalOffset = 0.0;

	double viewLength;

	int mazeSizeIncrement; 
	double wallThickness;

	Size screenSize;
	Size currentMazeSize;
	Size cellSize;

	Segment move;

	Canvas mazeDrawingCanvas, projectionDrawingCanvas;

	double map(double value, double istart, double istop, double ostart, double ostop);
	void keyHandler(double multiplier);
	bool levelCompleted();
	void newLevel();
	void placeCollectible(EntityState& collectible);
	void placeCollectibles(std::vector<EntityState>& entities);
	void renderMinimap();
	void renderProjection();
	void loadTextures();

	void init();
public:
	Game(Size windowSize, std::string windowTitle, double fov, int noRays, double viewLength, Size firstMazeSize, int mazeSizeIncrement, Size cellSize, double wallThickness, int collectiblesAmount);
	~Game();

	bool update(double elapsedTime);
	void render();

	void addWall(ObstacleState segment);
	void addWalls(std::vector<ObstacleState>& segments);

	void generateCollectible();
	void generateCollectibles(int amount);
	void addCollectible(EntityState collectible);
	void addCollectibles(std::vector<EntityState>& collectibles);
};