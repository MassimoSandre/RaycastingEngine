#include "Game.h"


void Game::keyHandler(double multiplier) {
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
	if (this->renderer.isKeyPressed(GLFW_KEY_SPACE)) {
		if (!this->jumping) {
			this->jumping = true;
			this->verticalOffset = 0.0f;
			this->verticalVelocity = 400.0f;
		}
	}
	if (move.length > 0) {
		this->player.tryMove(move, this->walls, multiplier);
	}
}

bool Game::levelCompleted() {
	return false;
}

void Game::newLevel() {
	this->renderer.setCanvasRealSize(MAZE_CANVAS,Coordinates{ (double)(this->currentMazeSize.x + 2) * this->cellSize.x ,  (double)(this->currentMazeSize.y + 2) * this->cellSize.y });
	this->generator.setSize(this->currentMazeSize);
	this->generator.generate();
	this->addWalls(generator.getWalls(this->cellSize, this->cellSize.toCoordinates() , this->wallThickness));
}

void Game::placeCollectible(std::shared_ptr<Entity>& e) {
	srand((unsigned int)time(NULL));

	int newx = rand() % (this->currentMazeSize.x - 1) + 1;
	int newy = rand() % (this->currentMazeSize.y - 1) + 1;

	e->center = {(double(newx)+0.5f)*this->cellSize.x,(double(newy) + 0.5f) * this->cellSize.y };
}

void Game::renderMinimap() {
	double angle;
	double step = 1;
	Coordinates p;
	this->renderer.drawCircle(Coordinates{ MINIMAP_RANGE, MINIMAP_RANGE }, MINIMAP_RANGE, RGB{0,0,0}, MAZE_CANVAS);
	for (std::shared_ptr<Segment> const& w : this->walls) {
		if (w->p1.distance(this->player.center) <= MINIMAP_RANGE) {
			if (w->p2.distance(this->player.center) <= MINIMAP_RANGE) {
				// both p1 and p2 are in range
				this->renderer.drawLine(
					w->p1.x - this->player.center.x + MINIMAP_RANGE, w->p1.y - this->player.center.y + MINIMAP_RANGE,
					w->p2.x - this->player.center.x + MINIMAP_RANGE, w->p2.y - this->player.center.y + MINIMAP_RANGE, 
					{255,0,0},
					MAZE_CANVAS);
			}
			else {
				// p1 is in range, p2 isn't
				angle = w->p1.getAngle(w->p2);
				p = w->p1;
				while (p.distance(this->player.center) <= MINIMAP_RANGE && p.distance(w->p1) < w->length) {
					this->renderer.drawPixel(p - this->player.center + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
					p.x += step * cos(angle);
					p.y -= step * sin(angle);
				}
			}
		}
		else if (w->p2.distance(this->player.center) <= MINIMAP_RANGE) {
			// p2 is in range, p1 isn't
			angle = w->p2.getAngle(w->p1);
			p = w->p2;
			while (p.distance(this->player.center) <= MINIMAP_RANGE && p.distance(w->p2) < w->length) {
				this->renderer.drawPixel(p - this->player.center + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
				p.x += step * cos(angle);
				p.y -= step * sin(angle);
			}
			
		}
		else if (!(w->length <= MINIMAP_RANGE &&
			w->p1.distance(this->player.center) >= 2 * MINIMAP_RANGE &&
			w->p2.distance(this->player.center) >= 2 * MINIMAP_RANGE)) {
			angle = w->p1.getAngle(w->p2);
			p = w->p1;
			while (p.distance(this->player.center) > MINIMAP_RANGE && p.distance(w->p1) < w->length) {
				p.x += step * cos(angle);
				p.y -= step * sin(angle);
			}
			while (p.distance(this->player.center) <= MINIMAP_RANGE && p.distance(w->p1) < w->length) {
				this->renderer.drawPixel(p - this->player.center - Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
				p.x += step * cos(angle);
				p.y -= step * sin(angle);
			}
		}
	}

	this->renderer.drawView(this->player.rays, {255,255,255}, this->player.center - Coordinates{MINIMAP_RANGE,MINIMAP_RANGE}, MAZE_CANVAS);
}

Game::Game(Size windowSize, std::string windowTitle, Coordinates playerStartingPosition, double playerStartingAngle,double fov, int noRays, double viewLength, Size firstMazeSize, int mazeSizeIncrement, Size cellSize, double wallThickness) :
	player(playerStartingPosition, fov, noRays, viewLength, playerStartingAngle),
	renderer(windowSize, "Maze"),
	generator() {
	this->mazeSizeIncrement = mazeSizeIncrement;
	this->screenSize = windowSize;
	this->cellSize = cellSize;
	this->wallThickness = wallThickness;
	this->currentMazeSize = firstMazeSize;

	mazeDrawingCanvas.drawingRect = Rect{ {20,20}, {MINIMAP_SIZE, MINIMAP_SIZE} };
	mazeDrawingCanvas.realSize = { MINIMAP_RANGE*2, MINIMAP_RANGE*2};
	projectionDrawingCanvas.drawingRect = Rect{ {0,0}, windowSize.toCoordinates()};
	projectionDrawingCanvas.realSize = windowSize.toCoordinates();
	
	this->renderer.addCanvas(mazeDrawingCanvas);
	this->renderer.addCanvas(projectionDrawingCanvas);

	this->newLevel();
}

Game::~Game() {}


bool Game::update(double elapsedTime) {
	this->keyHandler(elapsedTime);

	if (!this->pause) {
		double cursorXDelta = this->screenSize.x / 4 * 3 - this->renderer.getMousePosition().x;
		this->renderer.setMousePosition({ (double)this->screenSize.x / 4 * 3, (double)this->screenSize.y / 2 });
		this->player.rotate(0.003 * cursorXDelta);
	}

	if (this->jumping) {
		this->verticalOffset += this->verticalVelocity;
		this->verticalVelocity -= 15;
		if (this->verticalOffset <= 0) {
			this->jumping = false;
			this->verticalOffset = 0;
		}
	}

	if (this->levelCompleted()) {
		this->currentMazeSize.x += this->mazeSizeIncrement;
		this->currentMazeSize.y += this->mazeSizeIncrement;
		this->newLevel();
	}

	for (int i = 0; i < this->collectibles.size(); i++) {
		this->collectibles[i]->faceTo(this->player.center);
	}

	for (std::shared_ptr<Entity>& e : this->collectibles) {
		if (this->player.collideWithEntity(e)) {
			this->placeCollectible(e);
		}
	}

	this->player.update();
	this->player.cast(this->walls, this->collectibles);

	return !this->closing && !this->renderer.closing();
}
void Game::render() {
	this->renderer.drawProjection(this->player.getFixedDistances(), this->verticalOffset, PROJECTION_CANVAS);
	this->renderMinimap();

	//this->renderer.drawCollectibles(this->collectibles, RGB{ 255,255,0 }, MAZE_CANVAS);

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