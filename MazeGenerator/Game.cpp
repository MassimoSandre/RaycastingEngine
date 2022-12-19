#include "Game.h"

double Game::map(double value, double istart, double istop, double ostart, double ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}


void Game::keyHandler(double multiplier) {
	Segment move(this->player.state.position, this->player.state.position);

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
			this->cameraVerticalOffset = 0.0f;
			this->cameraVerticalVelocity = 400.0f;
		}
	}
	if (move.length > 0) {
		MovingEntity::tryMove(this->player.state,move, this->walls, multiplier);
	}
}

bool Game::levelCompleted() {
	return (this->player.state.position.x > double(this->currentMazeSize.x + 1) * this->cellSize.x &&
		this->player.state.position.y > double(this->currentMazeSize.y) * this->cellSize.y);
}

void Game::newLevel() {
	this->generator.setSize(this->currentMazeSize);
	this->generator.generate();
	
	this->walls.clear();
	this->walls = generator.getWalls(this->cellSize, this->cellSize.toCoordinates(), this->wallThickness, std::make_shared<Wall>(Obstacles::brickWall));

	this->player.state.position = { double(this->cellSize.x) / 2,3 * double(this->cellSize.y) / 2 };

	this->placeCollectibles(this->collectibles);
}

void Game::placeCollectible(EntityState& collectible) {
	srand((unsigned int)time(NULL));
	
	int newx = rand() % this->currentMazeSize.x + 1;
	int newy = rand() % this->currentMazeSize.y + 1;

	collectible.position = { (double(newx) + 0.5) * double(this->cellSize.x),(double(newy) + 0.5) * double(this->cellSize.y) };
	collectible.set(collectible.position, collectible.length, collectible.angle);
}
void Game::placeCollectibles(std::vector<EntityState>& entities) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < entities.size(); i++) {
		int newx = rand() % this->currentMazeSize.x + 1;
		int newy = rand() % this->currentMazeSize.y + 1;

		entities[i].position = {(double(newx) + 0.5) * double(this->cellSize.x),(double(newy) + 0.5) * double(this->cellSize.y)};
		entities[i].set(entities[i].position, entities[i].length, entities[i].angle);
	}
}
void Game::renderMinimap() {
	double angle;
	double step = double(MINIMAP_RANGE) / double(MINIMAP_SIZE/2);
	Coordinates p;
	this->renderer.drawCircle(Coordinates{ MINIMAP_RANGE, MINIMAP_RANGE }, MINIMAP_RANGE, RGB{0,0,0}, MAZE_CANVAS);
	for (int i = 0; i < this->walls.size(); i++) {
		for (int j = 0; j < this->walls[i].segments.size(); j++) {
			if (this->walls[i].segments[j].p1.distance(this->player.state.position) <= MINIMAP_RANGE) {
				if (this->walls[i].segments[j].p2.distance(this->player.state.position) <= MINIMAP_RANGE) {
					// both p1 and p2 are in range
					this->renderer.drawLine(
						this->walls[i].segments[j].p1.x - this->player.state.position.x + MINIMAP_RANGE, this->walls[i].segments[j].p1.y - this->player.state.position.y + MINIMAP_RANGE,
						this->walls[i].segments[j].p2.x - this->player.state.position.x + MINIMAP_RANGE, this->walls[i].segments[j].p2.y - this->player.state.position.y + MINIMAP_RANGE,
						{ 255,0,0 },
						MAZE_CANVAS);
				}
				else {
					// p1 is in range, p2 isn't
					angle = this->walls[i].segments[j].p1.getAngle(this->walls[i].segments[j].p2);
					p = this->walls[i].segments[j].p1;
					while (p.distance(this->player.state.position) <= MINIMAP_RANGE && p.distance(this->walls[i].segments[j].p1) < this->walls[i].segments[j].length) {
						this->renderer.drawPixel(p - this->player.state.position + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
						p.x += step * cos(angle);
						p.y -= step * sin(angle);
					}
				}
			}
			else if (this->walls[i].segments[j].p2.distance(this->player.state.position) <= MINIMAP_RANGE) {
				// p2 is in range, p1 isn't
				angle = this->walls[i].segments[j].p2.getAngle(this->walls[i].segments[j].p1);
				p = this->walls[i].segments[j].p2;
				while (p.distance(this->player.state.position) <= MINIMAP_RANGE && p.distance(this->walls[i].segments[j].p2) < this->walls[i].segments[j].length) {
					this->renderer.drawPixel(p - this->player.state.position + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
					p.x += step * cos(angle);
					p.y -= step * sin(angle);
				}

			}
			else if (!(this->walls[i].segments[j].length <= MINIMAP_RANGE &&
				this->walls[i].segments[j].p1.distance(this->player.state.position) >= 2 * MINIMAP_RANGE &&
				this->walls[i].segments[j].p2.distance(this->player.state.position) >= 2 * MINIMAP_RANGE)) {
				angle = this->walls[i].segments[j].p1.getAngle(this->walls[i].segments[j].p2);
				p = this->walls[i].segments[j].p1;
				while (p.distance(this->player.state.position) > MINIMAP_RANGE && p.distance(this->walls[i].segments[j].p1) < this->walls[i].segments[j].length) {
					p.x += step * cos(angle);
					p.y -= step * sin(angle);

				}
				while (p.distance(this->player.state.position) <= MINIMAP_RANGE && p.distance(this->walls[i].segments[j].p1) < this->walls[i].segments[j].length) {
					this->renderer.drawPixel(p - this->player.state.position + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, RGB{ 255,0,0 }, MAZE_CANVAS);
					p.x += step * cos(angle);
					p.y -= step * sin(angle);
				}
			}
		}
	}
	for (int i = 0; i < this->collectibles.size(); i++) {
		if (this->collectibles[i].position.distance(this->player.state.position) <= MINIMAP_RANGE - this->collectibles[i].length / 2) {
			this->renderer.drawCircle(this->collectibles[i].position - this->player.state.position + Coordinates{ MINIMAP_RANGE,MINIMAP_RANGE }, this->collectibles[i].length/2 , { 255,255,0 }, MAZE_CANVAS);
		}
	}
	
	this->renderer.drawView(this->player.rays, {255,255,255}, this->player.state.position - Coordinates{MINIMAP_RANGE,MINIMAP_RANGE}, MAZE_CANVAS);
}
void Game::renderProjection() {
	ViewInfo info = this->player.getFixedDistances();
	size_t len = info.size();

	double rectWidth = this->projectionDrawingCanvas.realSize.x / len;

	double rectHeight, wallHeight;
	double offset;

	double grey;
	Coordinates p;


	for (int i = 0; i < len; i++) {

		p.x = rectWidth * i;

		double d = DEFAULT_WALL_HEIGHT / this->projectionDrawingCanvas.realSize.y;

		wallHeight = (DEFAULT_WALL_HEIGHT + 2*this->cameraVerticalOffset) / d;

		int j = -int((wallHeight - this->projectionDrawingCanvas.realSize.y) * 2);
		while (j < this->projectionDrawingCanvas.realSize.y) {

			if (j < this->projectionDrawingCanvas.realSize.y / 2) {
				rectHeight = this->projectionDrawingCanvas.realSize.y - (2 * j);
			}
			else {
				rectHeight = this->projectionDrawingCanvas.realSize.y - (2 * (this->projectionDrawingCanvas.realSize.y - j));
			}

			double distance = std::min(double(DEFAULT_WALL_HEIGHT) / rectHeight, 100.0);
			wallHeight = (DEFAULT_WALL_HEIGHT + 2*this->cameraVerticalOffset) / distance;

			offset = (wallHeight - rectHeight) / 2;

			grey = this->map(pow(distance, 0.5), -10, pow(this->viewLength, 0.5), 200, 0);

			this->renderer.drawRect(Coordinates{ p.x,(double)j + int(offset) }, Coordinates{ rectWidth, double(CHUNK_SIZE + int(offset)) }, { int(grey),int(grey),int(grey) }, PROJECTION_CANVAS);

			j += CHUNK_SIZE;

		}

		for (int k = 0; k < info[i].size(); k++) {
			rectHeight = (info[i][k].height) / info[i][k].distance;
			wallHeight = (DEFAULT_WALL_HEIGHT + 2*(this->cameraVerticalOffset)) / info[i][k].distance;
			offset = (wallHeight - rectHeight) / 2;
			Size textureSize = info[i][k].element->texture.size;

			double normalPixelHeight = double(PIXEL_HEIGHT_REAL_HEIGHT_RATIO) * info[i][k].height;
			double pixelHeight = normalPixelHeight - (PIXEL_HEIGHT_REAL_HEIGHT_RATIO * (info[i][k].verticalOffset));

			grey = this->map(pow(info[i][k].distance, 0.8), 0, pow(this->viewLength, 0.8), 0.9, 0);

			p.y = (this->projectionDrawingCanvas.realSize.y - rectHeight ) / 2 + offset;

			int c = int(info[i][k].colOffset) - textureSize.x/2;
			if (c >= 0) {
				c = c % textureSize.x;
			}
			else {
				c = (textureSize.x - c % textureSize.x) % textureSize.x;
			}
			rectHeight = rectHeight / normalPixelHeight;

			for (int j = int(normalPixelHeight - pixelHeight); j < normalPixelHeight; j++) {
				RGBA color = info[i][k].element->texture.texture[c * textureSize.y + j % textureSize.y];

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

void Game::init() {
	Obstacles::load();	
	Entities::load();

	this->player.assignEntityState(Entities::player.getDefaultState());
}

Game::Game(Size windowSize, std::string windowTitle,double fov, int noRays, double viewLength, Size firstMazeSize, int mazeSizeIncrement, Size cellSize, double wallThickness, int collectiblesAmount) :
	player(fov, noRays, viewLength),
	renderer(windowSize, "Maze"),
	generator() {
	this->mazeSizeIncrement = mazeSizeIncrement;
	this->screenSize = windowSize;
	this->cellSize = cellSize;
	this->wallThickness = wallThickness;
	this->currentMazeSize = firstMazeSize;
	this->viewLength = viewLength;

	mazeDrawingCanvas.drawingRect = Rect{ {20,20}, {MINIMAP_SIZE, MINIMAP_SIZE} };
	mazeDrawingCanvas.realSize = { MINIMAP_RANGE*2, MINIMAP_RANGE*2};
	projectionDrawingCanvas.drawingRect = Rect{ {0,0}, windowSize.toCoordinates()};
	projectionDrawingCanvas.realSize = windowSize.toCoordinates();
	
	this->renderer.addCanvas(mazeDrawingCanvas);
	this->renderer.addCanvas(projectionDrawingCanvas);

	this->init();
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
		this->cameraVerticalOffset += this->cameraVerticalVelocity;
		this->cameraVerticalVelocity -= 30;
		if (this->cameraVerticalOffset <= 0) {
			this->jumping = false;
			this->cameraVerticalOffset = 0;
		}
	}

	if (this->levelCompleted()) {
		this->currentMazeSize.x += this->mazeSizeIncrement;
		this->currentMazeSize.y += this->mazeSizeIncrement;
		this->newLevel();
	}

	for (int i = 0; i < this->collectibles.size(); i++) {
		this->collectibles[i].faceTo(this->player.state.position);
	}

	for (EntityState& e : this->collectibles) {
		if (e.owner->collideWith(e, player.state.position)) {
			this->placeCollectible(e);
			this->player.inventory.addItemStack(ItemStack(e.owner, 1));
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

void Game::addWall(ObstacleState segment) {
	this->walls.push_back(segment);
}
void Game::addWalls(std::vector<ObstacleState>& segments) {
	for (int i = 0; i < segments.size(); i++) {
		this->walls.push_back(segments[i]);
	}
}

void Game::generateCollectible() {
	this->collectibles.push_back(Entities::coin.getDefaultState().with(Coordinates{0,0}, 16, 0.0));
}
void Game::generateCollectibles(int amount) {
	for (int i = 0; i < amount; i++) {
		this->generateCollectible();
	}
}
void Game::addCollectible(EntityState collectible) {
	this->collectibles.push_back(collectible);
}
void Game::addCollectibles(std::vector<EntityState>& collectibles) {
	for (int i = 0; i < collectibles.size(); i++) {
		this->collectibles.push_back(collectibles[i]);
	}
}