#include "Renderer.h"

#define DEFAULT_SCREEN_WIDTH_VALUE 120
#define DEFAULT_SCREEN_HEIGHT_VALUE	120

void Renderer::init() {
	if (!glfwInit())
		throw std::ios_base::failure("glfwInit failed");
	this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, this->windowTitle.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::ios_base::failure("window creation failed");
	}
	glfwMakeContextCurrent(window);

	Raycaster tCaster({ 400,400 }, 3.14 / 2, 100, 50, 0);
	caster = std::make_shared<Raycaster>(tCaster);
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
		this->caster->move({ 0,-1 });
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->caster->move({ -1,0 });
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->caster->move({ 0,1 });
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->caster->move({ 1,0 });
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		this->caster->follow(this->getMousePosition(), 2);
	}

	// WALLS
	this->drawSegments(this->walls, {255,0,0});

	// RAYCASTER
	this->caster->pointTo(this->getMousePosition());
	this->caster->cast(this->walls);
	this->drawView(this->caster);

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