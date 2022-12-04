#include "Renderer.h"

#define DEFAULT_SCREEN_WIDTH_VALUE 120
#define DEFAULT_SCREEN_HEIGHT_VALUE	120

#define RAYS_LENGTH 200


void Renderer::init() {
	if (!glfwInit())
		throw std::ios_base::failure("glfwInit failed");
	this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, this->windowTitle.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::ios_base::failure("window creation failed");
	}
	glfwMakeContextCurrent(window);

	Raycaster tCaster({ 400,400 }, 3.14 / 2, 30, RAYS_LENGTH, 0);
	caster = std::make_shared<Raycaster>(tCaster);

	this->caster->pointTo(this->getMousePosition());

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
	if (glfwGetKey(this->window,GLFW_KEY_W) == GLFW_PRESS) {
		this->caster->moveForward(1);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->caster->moveLeftward(1);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->caster->moveBackward(1);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->caster->moveRightward(1);
	}
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		this->caster->follow(this->getMousePosition(), 2);
	}

	int cursorXDelta = this->screenWidth / 4 * 3-this->getMousePosition().x;
	glfwSetCursorPos(this->window, this->screenWidth / 4 * 3, this->screenHeight / 2);

	// WALLS
	this->drawSegments(this->walls, {255,0,0});

	// RAYCASTER
	//this->caster->pointTo(this->getMousePosition());
	this->caster->rotate(0.003 * cursorXDelta);
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

void Renderer::drawRect(Coordinates topLeft, Size size) {
	this->drawRect(topLeft, size, { 255,255,255 });

}
void Renderer::drawRect(Coordinates topLeft, Size size, RGB color) {
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

void Renderer::drawProjection(std::vector<Coordinates> distances) {
	int len = distances.size();
	int rectWidth = (this->screenWidth / 2) / len;

	int minHeight = 100;
	int rectHeight;
	int grey;
	Coordinates p;

	for (int i = 0; i < len; i++) {
		if (distances[i].x == distances[i].y) continue;

		rectHeight = this->screenHeight - int(distances[i].x / float(RAYS_LENGTH) * float(this->screenHeight - minHeight));
		grey = 255 - int(distances[i].x / float(RAYS_LENGTH) * 235);

		p.x = this->screenWidth/2 + rectWidth * i;
		p.y = (this->screenHeight - rectHeight) / 2;

		this->drawRect(p, { rectWidth, rectHeight }, { grey,grey,grey });
	}
}