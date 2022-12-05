#include "Renderer.h"

float Renderer::scaleX(int x) {
	float result = x;
	result = result * (float(this->mazeDrawingSquare.size.x) / float(this->screenWidth));
	result += this->mazeDrawingSquare.topLeft.x;
	result = (float)2 * result / (this->mazeDrawingSquare.size.x + this->projectionDrawingSquare.size.x) - 1;
	return result;
}
float Renderer::scaleY(int y) {
	float result = y;
	result = result * (float(this->mazeDrawingSquare.size.y) / float(this->screenHeight));
	result += this->mazeDrawingSquare.topLeft.y;
	result = (float)2 * result / (this->mazeDrawingSquare.size.y) - 1;
	return result;
}

float Renderer::map(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}


Renderer::Renderer(Size mazeRealSize, std::string windowTitle, Rect mazeDrawingSquare, Rect projectionDrawingSquare) {
	this->screenWidth = mazeRealSize.x;
	this->screenHeight = mazeRealSize.y;
	this->windowTitle = windowTitle;
	this->mazeDrawingSquare = mazeDrawingSquare;
	this->projectionDrawingSquare = projectionDrawingSquare;
	this->init();
}

Renderer::~Renderer() {
	glfwTerminate();
}

void Renderer::init() {
	if (!glfwInit()) {
		throw std::ios_base::failure("glfw init failed");
	}
	this->window = glfwCreateWindow(this->mazeDrawingSquare.size.x + this->projectionDrawingSquare.size.x, this->projectionDrawingSquare.size.y, this->windowTitle.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::ios_base::failure("window creation failed");
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

Coordinates Renderer::getMousePosition() {
	double xpos, ypos;
	glfwGetCursorPos(this->window, &xpos, &ypos);

	return { float(xpos), float(ypos) };
}
void Renderer::setMousePosition(Coordinates position) {
	glfwSetCursorPos(this->window, position.x, position.y);
}

bool Renderer::isKeyPressed(int key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}
bool Renderer::leftClick() {
	return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}
bool Renderer::closing() {
	return glfwWindowShouldClose(this->window);
}

void Renderer::update() {
	glfwSwapBuffers(this->window);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawPixel(int x, int y) {
	this->drawPixel(x, y, { 255,255,255 });
}
void Renderer::drawPixel(int x, int y, RGB color) {
	float xf = this->scaleX(x);
	float yf = this->scaleY(y);

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
	float xf1 = this->scaleX(x1);
	float yf1 = this->scaleY(y1);

	float xf2 = this->scaleX(x2);
	float yf2 = this->scaleY(y2);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(xf1, -yf1);
	glVertex2f(xf2, -yf2);
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

void Renderer::drawView(std::vector<std::shared_ptr<Segment>> r, bool connect) {
	this->drawView(r, { 255,255,255 }, connect);
}
void Renderer::drawView(std::vector<std::shared_ptr<Segment>> r, RGB color, bool connect) {
	this->drawSegment(r[0]);
	this->drawSegment(r[r.size() - 1]);

	for (int i = 0; i < r.size() - 1; i++) {
		if (connect) {
			this->drawLine(r[i]->p2, r[i + 1]->p2, color);
		}
		else {
			this->drawPixel(r[i]->p2, color);
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

	float x1f = (float)2 * x1 / (this->mazeDrawingSquare.size.x + this->projectionDrawingSquare.size.x) - 1;
	float y1f = (float)2 * y1 / this->projectionDrawingSquare.size.y - 1;

	float x2f = (float)2 * x2 / (this->mazeDrawingSquare.size.x + this->projectionDrawingSquare.size.x) - 1;
	float y2f = (float)2 * y2 / this->projectionDrawingSquare.size.y - 1;

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
void Renderer::drawRect(Rect rect) {
	this->drawRect(rect.topLeft, rect.size, { 255,255,255 });
}
void Renderer::drawRect(Rect rect, RGB color) {
	this->drawRect(rect.topLeft, rect.size, color);
}

void Renderer::drawProjection(std::vector<RenderInfo> distances, float wallHeight) {
	int len = distances.size();

	const int CHUNK_SIZE = 4;
	const int WALL_PIXEL_HEIGHT = 64;
	const int IMAGE_HEIGHT = 16;
	const int IMAGE_WIDTH = 16;


	float rectWidth = this->projectionDrawingSquare.size.x / len;

	float rectHeight;
	float grey;
	Coordinates p;

	for (int i = 0; i < len; i++) {
		p.x = this->projectionDrawingSquare.size.x + rectWidth * i;

		
		for (int j = 0; j < this->projectionDrawingSquare.size.y/ CHUNK_SIZE; j++) {
			grey = this->map(pow(abs(j * CHUNK_SIZE - this->projectionDrawingSquare.size.y / 2),2), 0, pow(this->projectionDrawingSquare.size.y / 2,2), 0, 100);
			if (j < (this->projectionDrawingSquare.size.y / CHUNK_SIZE)/2) {
				rectHeight = this->projectionDrawingSquare.size.y - (2 * (j * CHUNK_SIZE)) ;
			}
			else {
				rectHeight = this->projectionDrawingSquare.size.y - (2 * (this->projectionDrawingSquare.size.y - (j * CHUNK_SIZE))) ;
			}

			double distance = std::min(wallHeight / rectHeight, distances[i].maxLength);
			
			grey = this->map(pow(distance, 0.5), 0, pow(distances[i].maxLength, .5), 160, 0);
			this->drawRect(Coordinates{p.x,(float)j*CHUNK_SIZE}, Coordinates{rectWidth, CHUNK_SIZE}, {int(grey),int(grey),int(grey)});
		}

		if (distances[i].distance == distances[i].maxLength) continue;

		rectHeight = wallHeight / distances[i].distance;
		grey = this->map(pow(distances[i].distance, 0.5), 0, pow(distances[i].maxLength, .5), 1, 0);

		p.y = (this->projectionDrawingSquare.size.y - rectHeight) / 2;

		int c = int(distances[i].colOffset);

		rectHeight = rectHeight / WALL_PIXEL_HEIGHT;

		for (int j = 0; j < WALL_PIXEL_HEIGHT; j++) {
			RGB color = this->wallTexture[c%IMAGE_WIDTH][j%IMAGE_HEIGHT];
			color.r = float(color.r)* grey;
			color.g = float(color.g) * grey;
			color.b = float(color.b) * grey;
			this->drawRect(p, Coordinates{ rectWidth, rectHeight }, color);
			
			p.y += rectHeight;
		}


	}
}