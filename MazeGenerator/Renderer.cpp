#include "Renderer.h"

double Renderer::scaleX(double x, int canvas) {
	double result = x;
	result = result * (this->drawingCanvas[canvas].drawingRect.size.x / this->drawingCanvas[canvas].realSize.x);
	result += this->drawingCanvas[canvas].drawingRect.topLeft.x;
	result = (double)2 * result / (this->drawingCanvas[0].drawingRect.size.x) - 1;
	return result;
}
double Renderer::scaleY(double y, int canvas) {
	double result = y;
	result = result * (this->drawingCanvas[canvas].drawingRect.size.y / this->drawingCanvas[canvas].realSize.y);
	result += this->drawingCanvas[canvas].drawingRect.topLeft.y;
	result = (double)2 * result / (this->drawingCanvas[0].drawingRect.size.y) - 1;
	return result;
}


Renderer::Renderer(Size windowSize, std::string windowTitle) {
	this->drawingCanvas.resize(1);
	this->drawingCanvas[0] = { {{0,0}, windowSize.toCoordinates()}, windowSize.toCoordinates() };

	this->windowTitle = windowTitle;

	this->init();
}

void Renderer::addCanvas(Canvas canvas) {
	this->drawingCanvas.push_back(canvas);
}

void Renderer::setCanvasRealSize(int canvasIndex, Coordinates realSize) {
	this->drawingCanvas[canvasIndex].realSize = realSize;
}

Renderer::~Renderer() {
	glfwTerminate();
}

void Renderer::init() {
	if (!glfwInit()) {
		throw std::ios_base::failure("glfw init failed");
	}
	this->window = glfwCreateWindow((int)this->drawingCanvas[0].realSize.x, (int)this->drawingCanvas[0].realSize.y, this->windowTitle.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::ios_base::failure("window creation failed");
	}
	glfwMakeContextCurrent(window);
}

Coordinates Renderer::getMousePosition() {
	double xpos, ypos;
	glfwGetCursorPos(this->window, &xpos, &ypos);

	return { double(xpos), double(ypos) };
}
void Renderer::setMousePosition(Coordinates position) {
	glfwSetCursorPos(this->window, position.x, position.y);
}

bool Renderer::isKeyPressed(int key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}
bool Renderer::isKeyReleased(int key) {
	return glfwGetKey(this->window, key) == GLFW_RELEASE;
}

bool Renderer::leftClickPressed() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}
bool Renderer::leftClickReleased() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE;
}
bool Renderer::middleClickClicked() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
}
bool Renderer::middleClickReleased() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE;
}
bool Renderer::rightClickPressed() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}
bool Renderer::rightClickReleased() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE;
}

bool Renderer::closing() {
	return glfwWindowShouldClose(this->window);
}

void Renderer::hideCursor() {
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Renderer::showCursor() {
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Renderer::update() {
	glfwSwapBuffers(this->window);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawPixel(double x, double y, int canvas) {
	this->drawPixel(x, y, { 255,255,255 }, canvas);
}
void Renderer::drawPixel(double x, double y, RGB color, int canvas) {
	double xf = this->scaleX(x, canvas);
	double yf = this->scaleY(y, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2d(xf, -yf);
	glEnd();
}
void Renderer::drawPixel(Coordinates p, int canvas) {
	this->drawPixel(p.x, p.y, { 255,255,255 }, canvas);
}
void Renderer::drawPixel(Coordinates p, RGB color, int canvas) {
	this->drawPixel(p.x, p.y, color, canvas);
}

void Renderer::drawLine(double x1, double y1, double x2, double y2, int canvas) {
	this->drawLine(x1, y1, x2, y2, { 255,255,255 }, canvas);
}
void Renderer::drawLine(double x1, double y1, double x2, double y2, RGB color, int canvas) {
	double xf1 = this->scaleX(x1, canvas);
	double yf1 = this->scaleY(y1, canvas);

	double xf2 = this->scaleX(x2, canvas);
	double yf2 = this->scaleY(y2, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2d(xf1, -yf1);
	glVertex2d(xf2, -yf2);
	glEnd();
}
void Renderer::drawLine(Coordinates p1, Coordinates p2, int canvas) {
	this->drawLine(p1.x, p1.y, p2.x, p2.y, { 255,255,255 }, canvas);
}
void Renderer::drawLine(Coordinates p1, Coordinates p2, RGB color, int canvas) {
	this->drawLine(p1.x, p1.y, p2.x, p2.y, color, canvas);
}
void Renderer::drawLine(Line l, int canvas) {
	this->drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, { 255,255,255 }, canvas);
}
void Renderer::drawLine(Line l, RGB color, int canvas) {
	this->drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, color, canvas);
}

void Renderer::drawLines(std::vector<Line>& lines, int canvas) {
	this->drawLines(lines, { 255,255,255 }, canvas);
}
void Renderer::drawLines(std::vector<Line>& lines, RGB color, int canvas) {
	for (Line const& l : lines) {
		this->drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, { 255,255,255 }, canvas);
	}
}

void Renderer::drawSegment(std::shared_ptr<Segment>& segment, int canvas) {
	this->drawLine(segment->p1.x, segment->p1.y, segment->p2.x, segment->p2.y, canvas);
}
void Renderer::drawSegments(std::vector<std::shared_ptr<Segment>>& segments, int canvas) {
	this->drawSegments(segments, { 255,255,255 }, canvas);
}
void Renderer::drawSegment(std::shared_ptr<Segment>& segment, RGB color, int canvas) {
	this->drawLine(segment->p1.x, segment->p1.y, segment->p2.x, segment->p2.y, color, canvas);
}
void Renderer::drawSegments(std::vector<std::shared_ptr<Segment>>& segments, RGB color, int canvas) {
	for (std::shared_ptr<Segment> s : segments) {
		this->drawLine(s->p1.x, s->p1.y, s->p2.x, s->p2.y, color, canvas);
	}
}

void Renderer::drawView(std::vector<std::shared_ptr<Segment>>& r, int canvas, bool connect) {
	this->drawView(r, { 255,255,255 }, connect, canvas);
}
void Renderer::drawView(std::vector<std::shared_ptr<Segment>>& r, RGB color, int canvas, bool connect) {
	this->drawSegment(r[0], canvas);
	this->drawSegment(r[r.size() - 1], canvas);

	for (int i = 0; i < r.size() - 1; i++) {
		if (connect) {
			this->drawLine(r[i]->p2, r[i + 1]->p2, color, canvas);
		}
		else {
			this->drawPixel(r[i]->p2, color, canvas);
		}
	}
}
void Renderer::drawView(std::vector<std::shared_ptr<Segment>>& r, RGB color, Coordinates center, int canvas, bool connect) {
	this->drawLine(r[0]->p1-center, r[0]->p2-center, canvas);
	this->drawLine(r[r.size() - 1]->p1 - center, r[r.size() - 1]->p2 - center, canvas);

	for (int i = 0; i < r.size() - 1; i++) {
		if (connect) {
			this->drawLine(r[i]->p2-center, r[i + 1]->p2-center, color, canvas);
		}
		else {
			this->drawPixel(r[i]->p2-center, color, canvas);
		}
	}
}

void Renderer::drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, int canvas) {
	this->drawTriangle(p1, p2, p3, { 255,255,255 }, canvas);
}
void Renderer::drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, RGB color, int canvas) {
	double xf1 = this->scaleX(p1.x, canvas);
	double yf1 = this->scaleY(p1.y, canvas);

	double xf2 = this->scaleX(p2.x, canvas);
	double yf2 = this->scaleY(p2.y, canvas);

	double xf3 = this->scaleX(p3.x, canvas);
	double yf3 = this->scaleY(p3.y, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_TRIANGLES);
	glVertex2d(xf1, -yf1);
	glVertex2d(xf2, -yf2);
	glVertex2d(xf3, -yf3);
	glEnd();
}

void Renderer::drawRect(Coordinates topLeft, Coordinates size, int canvas) {
	this->drawRect(topLeft, size, { 255,255,255 }, canvas);

}
void Renderer::drawRect(Coordinates topLeft, Coordinates size, RGB color, int canvas) {
	double x1 = topLeft.x,
		x2 = topLeft.x + size.x;
	double y1 = topLeft.y,
		y2 = topLeft.y + size.y;

	double x1f = this->scaleX(x1, canvas);
	double y1f = this->scaleY(y1, canvas);

	double x2f = this->scaleX(x2, canvas);
	double y2f = this->scaleY(y2, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_TRIANGLES);
	glVertex2d(x1f, -y1f);
	glVertex2d(x1f, -y2f);
	glVertex2d(x2f, -y2f);
	glVertex2d(x2f, -y1f);
	glVertex2d(x2f, -y2f);
	glVertex2d(x1f, -y1f);
	glEnd();
}
void Renderer::drawRect(Rect rect, int canvas) {
	this->drawRect(rect.topLeft, rect.size, { 255,255,255 }, canvas);
}
void Renderer::drawRect(Rect rect, RGB color, int canvas) {
	this->drawRect(rect.topLeft, rect.size, color, canvas);
}

void Renderer::drawCircle(Coordinates center, double radius, int canvas) {
	this->drawCircle(center, radius, { 255,255,255 }, canvas);
}
void Renderer::drawCircle(Coordinates center, double radius, RGB color, int canvas) {
	Coordinates last = center, next;
	last.y -= radius;
	double anglePerTriangle = (2 * 3.1415) / TRIANGLES_IN_CIRCLE;
	for (int i = 0; i <= TRIANGLES_IN_CIRCLE; i++) {
		next = center;
		next.x += radius * cos(anglePerTriangle * i);
		next.y -= radius * sin(anglePerTriangle * i);

		this->drawTriangle(center, last, next, color, canvas);

		last = next;
	}
}

void Renderer::drawQuadrilateral(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p4, int canvas) {
	this->drawQuadrilateral(p1, p2, p3, p4, { 255,255,255 }, canvas);
}
void Renderer::drawQuadrilateral(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p4, RGB color, int canvas) {
	p1.x = this->scaleX(p1.x, canvas);
	p1.y = this->scaleY(p1.y, canvas);
	p2.x = this->scaleX(p2.x, canvas);
	p2.y = this->scaleY(p2.y, canvas);
	p3.x = this->scaleX(p3.x, canvas);
	p3.y = this->scaleY(p3.y, canvas);
	p4.x = this->scaleX(p4.x, canvas);
	p4.y = this->scaleY(p4.y, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_TRIANGLES);
	glVertex2d(p1.x, -p1.y);
	glVertex2d(p2.x, -p2.y);
	glVertex2d(p3.x, -p3.y);
	glVertex2d(p3.x, -p3.y);
	glVertex2d(p4.x, -p4.y);
	glVertex2d(p2.x, -p2.y);
	glEnd();
}