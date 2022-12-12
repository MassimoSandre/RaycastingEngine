#include "Game.h"


double Game::map(double value, double istart, double istop, double ostart, double ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}


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
	if (this->renderer.isKeyPressed(GLFW_KEY_P) && this->canTogglePause) {
		this->pause = !this->pause;		
		this->canTogglePause = false;
	}
	if (this->renderer.isKeyReleased(GLFW_KEY_P)) {
		this->canTogglePause = true;
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
	return (this->player.center.x > double(this->currentMazeSize.x + 1) * this->cellSize.x &&
		this->player.center.y > double(this->currentMazeSize.y) * this->cellSize.y);
}

void Game::newLevel() {
	//this->renderer.setCanvasRealSize(MAZE_CANVAS,Coordinates{ (double)(this->currentMazeSize.x + 2) * this->cellSize.x ,  (double)(this->currentMazeSize.y + 2) * this->cellSize.y });
	this->generator.setSize(this->currentMazeSize);
	this->generator.generate();
	this->walls.clear();
	this->addWalls(generator.getWalls(this->cellSize, this->cellSize.toCoordinates() , this->wallThickness));
	this->player.center = { double(this->cellSize.x) / 2,3 * double(this->cellSize.y) / 2 };

	this->placeCollectibles(this->collectibles);
}

void Game::placeCollectible(std::shared_ptr<Entity>& e) {
	srand((unsigned int)time(NULL));
	
	int newx = rand() % this->currentMazeSize.x + 1;
	int newy = rand() % this->currentMazeSize.y + 1;

	e->center = { (double(newx) + 0.5) * double(this->cellSize.x),(double(newy) + 0.5) * double(this->cellSize.y) };
}
void Game::placeCollectibles(std::vector<std::shared_ptr<Entity>>& entities) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < entities.size(); i++) {
		int newx = rand() % this->currentMazeSize.x + 1;
		int newy = rand() % this->currentMazeSize.y + 1;

		entities[i]->center = {(double(newx) + 0.5) * double(this->cellSize.x),(double(newy) + 0.5) * double(this->cellSize.y)};
	}
}
void Game::renderMinimap() {
	double angle;
	double step = double(MINIMAP_RANGE) / double(MINIMAP_SIZE/2);
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
			while (p.distance(this->player.center) <= MINIMAP_RANGE &&  p.distance(w->p1) < w->length) {
				this->renderer.drawPixel(p - this->player.center + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
				p.x += step * cos(angle);
				p.y -= step * sin(angle);
			}
		}
	}
	for (std::shared_ptr<Entity> const& e : this->collectibles) {
		if (e->center.distance(this->player.center) <= MINIMAP_RANGE - e->length/2 ) {
			this->renderer.drawCircle(e->center - this->player.center + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, e->length/2 , { 255,255,0 }, MAZE_CANVAS);
		}
	}
	
	this->renderer.drawView(this->player.rays, {255,255,255}, this->player.center - Coordinates{MINIMAP_RANGE,MINIMAP_RANGE}, MAZE_CANVAS);
}
void Game::renderProjection() {
	//this->renderer.drawProjection(this->player.getFixedDistances(), this->verticalOffset, PROJECTION_CANVAS);
	RenderingInfo info = this->player.getFixedDistances();
	size_t len = info.size();

	double rectWidth = this->projectionDrawingCanvas.realSize.x / len;

	double rectHeight, wallHeight;
	double offset;

	double grey;
	Coordinates p;


	for (int i = 0; i < len; i++) {

		p.x = rectWidth * i;

		double d = DEFAULT_WALL_HEIGHT / this->projectionDrawingCanvas.realSize.y;

		wallHeight = (DEFAULT_WALL_HEIGHT + this->verticalOffset) / d;

		int j = -int((wallHeight - this->projectionDrawingCanvas.realSize.y) * 2);
		while (j < this->projectionDrawingCanvas.realSize.y) {

			if (j < this->projectionDrawingCanvas.realSize.y / 2) {
				rectHeight = this->projectionDrawingCanvas.realSize.y - (2 * j);
			}
			else {
				rectHeight = this->projectionDrawingCanvas.realSize.y - (2 * (this->projectionDrawingCanvas.realSize.y - j));
			}

			double distance = std::min(double(DEFAULT_WALL_HEIGHT) / rectHeight, 100.0);
			wallHeight = (DEFAULT_WALL_HEIGHT + this->verticalOffset) / distance;

			offset = (wallHeight - rectHeight) / 2;

			grey = this->map(pow(distance, 0.5), -10, pow(100, 0.5), 200, 0);

			//grey = this->map(pow(abs(this->projectionDrawingSquare.size.y / 2 - j * CHUNK_SIZE), 0.8), 0, pow(this->projectionDrawingSquare.size.y / 2, 0.8), 0, 180);

			this->renderer.drawRect(Coordinates{ p.x,(double)j + int(offset) }, Coordinates{ rectWidth, double(CHUNK_SIZE + int(offset)) }, { int(grey),int(grey),int(grey) }, PROJECTION_CANVAS);

			j += CHUNK_SIZE;

		}

		for (int k = 0; k < info[i].size(); k++) {
			rectHeight = info[i][k].height / info[i][k].distance;
			wallHeight = (DEFAULT_WALL_HEIGHT + this->verticalOffset) / info[i][k].distance;
			offset = (wallHeight - rectHeight) / 2;
			Size textureSize = this->textures[info[i][k].textureId]->size;
			int pixelHeight = int(PIXEL_HEIGHT_REAL_HEIGHT_RATIO * info[i][k].height);

			grey = this->map(pow(info[i][k].distance, 0.5), 0, pow(100 /*TO DE-HARDCODE*/, .5), 1, 0);

			p.y = (this->projectionDrawingCanvas.realSize.y - rectHeight) / 2 + offset;


			int c = int(info[i][k].colOffset) % (textureSize.x / 2) + textureSize.x / 2;

			rectHeight = rectHeight / pixelHeight;

			for (int j = 0; j < pixelHeight; j++) {
				RGBA color = this->textures[info[i][k].textureId]->texture[(c % textureSize.x) * textureSize.y + j % textureSize.y];

				if (color.a != 0) {
					color.r = int(double(color.r) * grey);
					color.g = int(double(color.g) * grey);
					color.b = int(double(color.b) * grey);
					this->renderer.drawRect(p, Coordinates{ rectWidth , rectHeight }, color.toRGB(), PROJECTION_CANVAS);
				}

				p.y += rectHeight;
			}
		}
	}
}

void Game::loadTextures() {
	this->textures.push_back(std::make_shared<Texture>("textures/wallAlt.texture"));
	this->textures.push_back(std::make_shared<Texture>("textures/collectible.texture"));
}

Game::Game(Size windowSize, std::string windowTitle, double playerStartingAngle,double fov, int noRays, double viewLength, Size firstMazeSize, int mazeSizeIncrement, Size cellSize, double wallThickness, int collectiblesAmount) :
	player({0,0}, fov, noRays, viewLength, playerStartingAngle),
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

	this->loadTextures();
	this->generateCollectibles(collectiblesAmount);
	this->newLevel();

}

Game::~Game() {}


bool Game::update(double elapsedTime) {
	this->keyHandler(elapsedTime);

	if (!this->pause) {
		double cursorXDelta = this->screenSize.x / 2 - this->renderer.getMousePosition().x;
		this->renderer.setMousePosition({ (double)this->screenSize.x / 2, (double)this->screenSize.y / 2 });
		this->player.rotate(0.003 * cursorXDelta);
		this->renderer.hideCursor();
	}
	else {
		this->renderer.showCursor();
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
	this->renderProjection();
	this->renderMinimap();

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

void Game::generateCollectible() {
	this->collectibles.push_back(std::make_shared<Entity>(Coordinates{0,0}, this->cellSize.x / 3, 0.0));
}
void Game::generateCollectibles(int amount) {
	for (int i = 0; i < amount; i++) {
		this->collectibles.push_back(std::make_shared<Entity>(Coordinates{ 0,0 }, this->cellSize.x / 3, 0.0));
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