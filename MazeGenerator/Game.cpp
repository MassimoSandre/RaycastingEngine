#include "Game.h"

#define MOVE_CHECK_DISTANCE 4
#define MOVE_DISTANCE 1 

void Game::tryMove(Segment move, float multiplier) {
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
		moveX.setLength(MOVE_DISTANCE * moveXCoef * multiplier);
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
		moveY.setLength(MOVE_DISTANCE * moveYCoef * multiplier);
		this->player.applyMove(moveY);
	}
}

void Game::keyHandler(float multiplier) {
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
		this->tryMove(move, multiplier);
	}
}

Game::Game(int nSquare, int windowSquareSize, Size mazeAreaSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle,double fov, int noRays, float viewLength) :
	player(playerStartingPosition, fov, noRays, viewLength, playerStartingAngle),
	renderer(mazeAreaSize, "Maze", Rect{ {0,0},{(float)windowSquareSize,(float)windowSquareSize} }, Rect{ {(float)windowSquareSize,0},{(float)windowSquareSize,(float)windowSquareSize} }) {
	
	this->screenSize = { nSquare * windowSquareSize, windowSquareSize };
}

Game::~Game() {}


bool Game::update(float elapsedTime) {
	this->keyHandler(elapsedTime);

	if (!this->pause) {
		float cursorXDelta = this->screenSize.x / 4 * 3 - this->renderer.getMousePosition().x;
		this->renderer.setMousePosition({ (float)this->screenSize.x / 4 * 3, (float)this->screenSize.y / 2 });
		this->player.rotate(0.003 * cursorXDelta);
	}

	for (int i = 0; i < this->collectibles.size(); i++) {
		this->collectibles[i]->faceTo(this->player.center);
	}

	this->player.cast(this->walls, this->collectibles);

	return !this->closing && !this->renderer.closing();
}
void Game::render() {
	this->renderer.drawSegments(this->walls, { 255,0,0 });
	this->renderer.drawCollectibles(this->collectibles, RGB{ 255,255,0 });
	//this->renderer.drawSegment(std::make_shared<Segment>(*this->collectibles[0]), {255,255,0});
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

void Game::addCollectible(std::shared_ptr<Entity> collectible) {
	this->collectibles.push_back(collectible);
}
void Game::addCollectibles(std::vector<std::shared_ptr<Entity>> collectibles) {
	for (int i = 0; i < collectibles.size(); i++) {
		this->collectibles.push_back(collectibles[i]);
	}
}