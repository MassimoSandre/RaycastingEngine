#include "Game.h"

#define MOVE_CHECK_DISTANCE 4
#define MOVE_DISTANCE 1 

void Game::tryMove(Segment move) {
	Segment moveX(move.p1, { move.p2.x, move.p1.y });
	Segment moveY(move.p1, { move.p1.x, move.p2.y });

	float moveXCoef = moveX.length / move.length;
	float moveYCoef = moveY.length / move.length;

	bool possible = true;
	for (std::shared_ptr<Segment> w : this->walls) {
		IntersectionInfo info = moveX.getIntersection(w);

		if (info.intersection.x != -1 || info.intersection.y != -1) {
			float d1 = info.intersection.distance(moveX.p1);

			float d2 = info.intersection.distance(moveX.p2);

			if (d1 > MOVE_CHECK_DISTANCE) continue;


			if (d2 > MOVE_CHECK_DISTANCE) continue;
			possible = false;
			break;
		}

	}
	if (possible) {
		moveX.setLength(MOVE_DISTANCE*moveXCoef);
		this->player.applyMove(moveX);
		moveY.translate({ float(moveX.p1.x > moveX.p2.x ? -moveX.length : +moveX.length) , 0 });
	}

	possible = true;
	for (std::shared_ptr<Segment> w : this->walls) {
		IntersectionInfo info = moveY.getIntersection(w);

		if (info.intersection.x != -1 || info.intersection.y != -1) {
			float d1 = info.intersection.distance(moveY.p1);

			float d2 = info.intersection.distance(moveY.p2);

			if (d1 > MOVE_CHECK_DISTANCE) continue;


			if (d2 > MOVE_CHECK_DISTANCE) continue;
			possible = false;
			break;
		}

	}
	if (possible) {
		moveY.setLength(MOVE_DISTANCE * moveYCoef);
		this->player.applyMove(moveY);
	}
}

void Game::keyHandler() {
	Segment move(this->player.center, this->player.center);

	if (this->renderer.isKeyPressed(GLFW_KEY_W)) {
		move.add(this->player.moveForward(1));	
	}
	if (this->renderer.isKeyPressed(GLFW_KEY_A)) {
		move.add(this->player.moveLeftward(1));
	}
	if (this->renderer.isKeyPressed(GLFW_KEY_S)) {
		move.add(this->player.moveBackward(1));
	}
	if (this->renderer.isKeyPressed(GLFW_KEY_D)) {
		move.add(this->player.moveRightward(1));
	}
	if (this->renderer.isKeyPressed(GLFW_KEY_ESCAPE)) {
		this->closing = true;
	}
	if (this->renderer.isKeyPressed(GLFW_KEY_P)) {
		this->pause = !this->pause;		
	}

	if (move.length > 0) {
		move.setLength(MOVE_CHECK_DISTANCE);
		this->tryMove(move);
	}
}

Game::Game(Size windowSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle,double fov, double noRays, double viewLength) :
	player(playerStartingPosition, fov, noRays, viewLength, playerStartingAngle),
	renderer(1600, 800, "Maze") {
	
	this->screenSize = windowSize;
}

Game::~Game() {}


bool Game::update() {
	this->keyHandler();

	if (!this->pause) {
		int cursorXDelta = this->screenSize.x / 4 * 3 - this->renderer.getMousePosition().x;
		this->renderer.setMousePosition({ (float)this->screenSize.x / 4 * 3, (float)this->screenSize.y / 2 });
		this->player.rotate(0.003 * cursorXDelta);
	}

	// PLAYER
	this->player.cast(this->walls);

	return !this->closing && !this->renderer.closing();
}
void Game::render() {
	// WALLS
	this->renderer.drawSegments(this->walls, { 255,0,0 });

	// PLAYER
	this->renderer.drawView(this->player.rays);
	this->renderer.drawProjection(this->player.getFixedDistances());

	this->renderer.update();
}

void Game::addWall(std::shared_ptr<Segment> segment) {
	this->walls.push_back(segment);
}
void Game::addWalls(std::vector<std::shared_ptr<Segment>> segments) {
	for (int i = 0; i < segments.size(); i++) {
		this->walls.push_back(segments[i]);
	}
}