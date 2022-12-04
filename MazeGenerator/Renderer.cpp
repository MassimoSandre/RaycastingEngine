#include "Renderer.h"

#include "Wall.h"

#define DEFAULT_SCREEN_WIDTH_VALUE 120
#define DEFAULT_SCREEN_HEIGHT_VALUE	120

#define RAYS_LENGTH 200
#define N_RAYS 200

#define PI 3.1415

#define FOV PI/2

#define MOVE_CHECK_DISTANCE 4
#define MOVE_DISTANCE 1 

void Renderer::init() {
	if (!glfwInit())
		throw std::ios_base::failure("glfwInit failed");
	this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, this->windowTitle.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::ios_base::failure("window creation failed");
	}
	glfwMakeContextCurrent(window);

	Raycaster tCaster({ 410,410 }, FOV, N_RAYS, RAYS_LENGTH, 0);
	caster = std::make_shared<Raycaster>(tCaster);

	this->caster->pointTo(this->getMousePosition());

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

float Renderer::map(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

Renderer::Renderer() {
	this->screenWidth = DEFAULT_SCREEN_WIDTH_VALUE;
	this->screenHeight = DEFAULT_SCREEN_HEIGHT_VALUE;
	this->windowTitle = "Window";
	this->init();
}

Renderer::Renderer(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->windowTitle = "Window";
	this->init();
}

Renderer::Renderer(int screenWidth, int screenHeight, std::string windowTitle) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->windowTitle = windowTitle;
	this->init();
}

Renderer::~Renderer() {
	glfwTerminate();
}

Coordinates Renderer::getMousePosition() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { float(xpos), float(ypos) };
}

bool Renderer::update() {
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		move = this->caster->moveForward(MOVE_CHECK_DISTANCE);
		bool possible = true;
		for (std::shared_ptr<Segment> w : this->walls) {
			IntersectionInfo info = move.getIntersection(w);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				float d1 = info.intersection.distance(move.p1);

				float d2 = info.intersection.distance(move.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;


				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}

		}
		if (possible) {
			move.setLength(MOVE_DISTANCE);
			this->caster->applyMove(move);
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		move = this->caster->moveLeftward(1);
		bool possible = true;
		for (std::shared_ptr<Segment> w : this->walls) {
			IntersectionInfo info = move.getIntersection(w);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				float d1 = info.intersection.distance(move.p1);

				float d2 = info.intersection.distance(move.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;


				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}

		}
		if (possible) {
			move.setLength(MOVE_DISTANCE);
			this->caster->applyMove(move);
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		move = this->caster->moveBackward(1);
		bool possible = true;
		for (std::shared_ptr<Segment> w : this->walls) {
			IntersectionInfo info = move.getIntersection(w);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				float d1 = info.intersection.distance(move.p1);

				float d2 = info.intersection.distance(move.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;


				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}

		}
		if (possible) {
			move.setLength(MOVE_DISTANCE);
			this->caster->applyMove(move);
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		move = this->caster->moveRightward(1);
		bool possible = true;
		for (std::shared_ptr<Segment> w : this->walls) {
			IntersectionInfo info = move.getIntersection(w);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				float d1 = info.intersection.distance(move.p1);

				float d2 = info.intersection.distance(move.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;


				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}

		}
		if (possible) {
			move.setLength(MOVE_DISTANCE);
			this->caster->applyMove(move);
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		move = this->caster->follow(this->getMousePosition(), 2);
		bool possible = true;
		for (std::shared_ptr<Segment> w : this->walls) {
			IntersectionInfo info = move.getIntersection(w);

			if (info.intersection.x != -1 || info.intersection.y != -1) {
				float d1 = info.intersection.distance(move.p1);

				float d2 = info.intersection.distance(move.p2);

				if (d1 > MOVE_CHECK_DISTANCE) continue;


				if (d2 > MOVE_CHECK_DISTANCE) continue;
				possible = false;
				break;
			}

		}
		if (possible) {
			move.setLength(MOVE_DISTANCE);
			this->caster->applyMove(move);
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS) {
		this->pause = !this->pause;
		glfwSetCursorPos(this->window, this->screenWidth / 4 * 3, this->screenHeight / 2);
	}

	if (!this->pause) {
		int cursorXDelta = this->screenWidth / 4 * 3 - this->getMousePosition().x;
		glfwSetCursorPos(this->window, this->screenWidth / 4 * 3, this->screenHeight / 2);
		this->caster->rotate(0.003 * cursorXDelta);
	}

	// WALLS
	this->drawSegments(this->walls, {255,0,0});

	// RAYCASTER
	//this->caster->pointTo(this->getMousePosition());
	this->caster->cast(this->walls);
	this->drawView(this->caster);
	this->drawProjection(this->caster->getFixedDistances());

	glfwSwapBuffers(this->window);

	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);

	return !glfwWindowShouldClose(this->window);
}

void Renderer::addWall(std::shared_ptr<Segment> segment) {
	this->walls.push_back(segment);
}
void Renderer::addWalls(std::vector<std::shared_ptr<Segment>> segments) {
	for (int i = 0; i < segments.size(); i++) {
		this->walls.push_back(segments[i]);
	}
}

void Renderer::drawPixel(int x, int y) {
	this->drawPixel(x, y, { 255,255,255 });
}
void Renderer::drawPixel(int x, int y, RGB color) {
	float xf = (float)2 * x / this->screenWidth - 1;
	float yf = (float)2 * y / this->screenHeight - 1;

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2f(xf, -yf);
	glEnd();
}
void Renderer::drawPixel(Coordinates p) {
	this->drawPixel(p.x, p.y, { 255,255,255 });
}
void Renderer::drawPixel(Coordinates p, RGB color) {
	this->drawPixel(p.x, p.y, color);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
	this->drawLine(x1, y1, x2, y2, { 255,255,255 });
}
void Renderer::drawLine(int x1, int y1, int x2, int y2, RGB color) {
	float x1f = (float)2 * x1 / this->screenWidth - 1;
	float y1f = (float)2 * y1 / this->screenHeight - 1;

	float x2f = (float)2 * x2 / this->screenWidth - 1;
	float y2f = (float)2 * y2 / this->screenHeight - 1;

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(x1f, -y1f);
	glVertex2f(x2f, -y2f);
	glEnd();
}
void Renderer::drawLine(Coordinates p1, Coordinates p2) {
	this->drawLine(p1.x, p1.y, p2.x, p2.y, { 255,255,255 });
}
void Renderer::drawLine(Coordinates p1, Coordinates p2, RGB color) {
	this->drawLine(p1.x, p1.y, p2.x, p2.y, color);
}
void Renderer::drawLine(Line l) {
	this->drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, { 255,255,255 });
}
void Renderer::drawLine(Line l, RGB color) {
	this->drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, color);
}

void Renderer::drawLines(std::vector<Line> lines) {
	this->drawLines(lines, { 255,255,255 });
}
void Renderer::drawLines(std::vector<Line> lines, RGB color) {
	for (Line l : lines) {
		this->drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, { 255,255,255 });
	}
}

void Renderer::drawSegment(std::shared_ptr<Segment> segment) {
	this->drawLine(segment->p1.x, segment->p1.y, segment->p2.x, segment->p2.y);
}
void Renderer::drawSegments(std::vector<std::shared_ptr<Segment>> segments) {
	this->drawSegments(segments, { 255,255,255 });
}
void Renderer::drawSegment(std::shared_ptr<Segment> segment, RGB color) {
	this->drawLine(segment->p1.x, segment->p1.y, segment->p2.x, segment->p2.y, color);
}
void Renderer::drawSegments(std::vector<std::shared_ptr<Segment>> segments, RGB color) {
	for (std::shared_ptr<Segment> s : segments) {
		this->drawLine(s->p1.x, s->p1.y, s->p2.x, s->p2.y, color);
	}
}

void Renderer::drawView(std::shared_ptr<Raycaster> r, bool connect) {
	this->drawView(r, { 255,255,255 }, connect);
}
void Renderer::drawView(std::shared_ptr<Raycaster> r, RGB color, bool connect) {
	this->drawSegment(r->rays[0]);
	this->drawSegment(r->rays[r->rays.size() - 1]);

	for (int i = 0; i < r->rays.size() - 1; i++) {
		if (connect) {
			this->drawLine(r->rays[i]->p2, r->rays[i + 1]->p2, color);
		}
		else {
			this->drawPixel(r->rays[i]->p2, color);
		}
	}
}

void Renderer::drawRect(Coordinates topLeft, Coordinates size) {
	this->drawRect(topLeft, size, { 255,255,255 });

}
void Renderer::drawRect(Coordinates topLeft, Coordinates size, RGB color) {
	int x1 = topLeft.x,
		x2 = topLeft.x + size.x;
	int y1 = topLeft.y,
		y2 = topLeft.y + size.y;

	float x1f = (float)2 * x1 / this->screenWidth - 1;
	float y1f = (float)2 * y1 / this->screenHeight - 1;

	float x2f = (float)2 * x2 / this->screenWidth - 1;
	float y2f = (float)2 * y2 / this->screenHeight - 1;

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_TRIANGLES);
	glVertex2f(x1f, -y1f);
	glVertex2f(x1f, -y2f);
	glVertex2f(x2f, -y2f);
	glVertex2f(x2f, -y1f);
	glVertex2f(x2f, -y2f);
	glVertex2f(x1f, -y1f);
	glEnd();
}

void Renderer::drawProjection(std::vector<RenderInfo> distances) {
	int len = distances.size();
	float rectWidth = (this->screenWidth / 2) / len;

	float minHeight = 300;
	float maxHeight = float(this->screenHeight) *1.0;
	float rectHeight;
	float grey;
	Coordinates p;

	for (int i = 0; i < len; i++) {
		p.x = this->screenWidth / 2 + rectWidth * i;

		const int CHUNK_SIZE = 4;
		for (int j = 0; j < this->screenHeight/ CHUNK_SIZE; j++) {
			grey = this->map(pow(abs(j * CHUNK_SIZE - this->screenHeight / 2),2), 0, pow(this->screenHeight / 2,2), 0, 100);
			this->drawRect(Coordinates{p.x,(float)j*CHUNK_SIZE}, Coordinates{rectWidth, CHUNK_SIZE}, {int(grey),int(grey),int(grey)});
		}

		if (distances[i].distance == distances[i].maxLength) continue;

		rectHeight =  maxHeight - distances[i].distance / float(RAYS_LENGTH) * float(maxHeight - minHeight);//(this->map(pow(distances[i].distance,1), 0, pow(distances[i].maxLength,1), maxHeight, minHeight));
		grey = this->map(pow(distances[i].distance, 0.5), 0, pow(distances[i].maxLength, .5), 1, 0);

		p.y = (this->screenHeight - rectHeight) / 2;

		int c = (int(distances[i].colOffset) % 32);

		const int IMAGE_HEIGHT = 64;

		rectHeight = rectHeight / IMAGE_HEIGHT;

		for (int j = 0; j < IMAGE_HEIGHT; j++) {
			RGB color = this->wallTexture[c%16][j%16];
			color.r = float(color.r)* grey;
			color.g = float(color.g) * grey;
			color.b = float(color.b) * grey;
			this->drawRect(p, Coordinates{ rectWidth, rectHeight }, color);
			
			p.y += rectHeight;
		}


	}
}