#include "Renderer.h"
#include "Entity.h"

#define TRIANGLES_IN_CIRCLE 6

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


Texture* Renderer::getTexture(RayType type) {
	switch (type) {
	case Generic:
		return NULL;
	case Obstacle:
		return &(this->wallTexture);
	case View:
		return NULL;
	case EntitySegment:
		return &(this->entityTexture);
	default:
		return NULL;
	}
}

float Renderer::getHeight(RayType type) {
	switch (type) {
	case Generic:
		return 0;
	case Obstacle:
		return 8000;
	case View:
		return 0;
	case EntitySegment:
		return 4000;
	default:
		return 0;
	}
}


Renderer::Renderer(Size mazeRealSize, std::string windowTitle, Rect mazeDrawingSquare, Rect projectionDrawingSquare) :
	wallTexture("textures/brickwall.texture"), 
	entityTexture("textures/collectible.texture") {
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

void Renderer::drawCollectible(std::shared_ptr<Entity> collectible) {
	this->drawCollectible(collectible, { 255,255,255 });
}
void Renderer::drawCollectibles(std::vector<std::shared_ptr<Entity>> collectibles) {
	this->drawCollectibles(collectibles, { 255,255,255 });
}
void Renderer::drawCollectible(std::shared_ptr<Entity> collectible, RGB color) {
	this->drawCircle(collectible->center, collectible->length / 2, color);
}
void Renderer::drawCollectibles(std::vector<std::shared_ptr<Entity>> collectibles, RGB color) {
	for (int i = 0; i < collectibles.size(); i++) {
		this->drawCircle(collectibles[i]->center, collectibles[i]->length / 2, color);
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

void Renderer::drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3) {
	this->drawTriangle(p1, p2, p3, { 255,255,255 });
}
void Renderer::drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, RGB color) {
	float xf1 = this->scaleX(p1.x);
	float yf1 = this->scaleY(p1.y);

	float xf2 = this->scaleX(p2.x);
	float yf2 = this->scaleY(p2.y);

	float xf3 = this->scaleX(p3.x);
	float yf3 = this->scaleY(p3.y);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_TRIANGLES);
	glVertex2f(xf1, -yf1);
	glVertex2f(xf2, -yf2);
	glVertex2f(xf3, -yf3);
	glEnd();
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

void Renderer::drawCircle(Coordinates center, float radius) { 
	this->drawCircle(center, radius, { 255,255,255 });
}
void Renderer::drawCircle(Coordinates center, float radius, RGB color) {
	Coordinates last = center, next;
	last.y -= radius;
	double anglePerTriangle = (2 * 3.1415) / TRIANGLES_IN_CIRCLE;
	for (int i = 0; i <= TRIANGLES_IN_CIRCLE; i++) {
		next = center;
		next.x += radius * cos(anglePerTriangle * i);
		next.y -= radius * sin(anglePerTriangle * i);

		this->drawTriangle(center, last, next, color);

		last = next;
	}
}

void Renderer::drawProjection(RenderingInfo info, float cameraVerticalOffset) {
	int len = info.size();

	const int CHUNK_SIZE = 4;
	const int WALL_PIXEL_HEIGHT = 128;
	const int IMAGE_HEIGHT = 16;
	const int IMAGE_WIDTH = 16;


	float rectWidth = this->projectionDrawingSquare.size.x / len;

	float rectHeight, wallHeight;
	float offset;
	
	float grey;
	Coordinates p;


	for (int i = 0; i < len; i++) {
		
		p.x = this->projectionDrawingSquare.topLeft.x + rectWidth * i;

		float d = this->getHeight(Obstacle) / this->projectionDrawingSquare.size.y;
		
		wallHeight = (this->getHeight(Obstacle) + cameraVerticalOffset) / d;
		
		int j = - (wallHeight - this->projectionDrawingSquare.size.y)*2;
		while (j < this->projectionDrawingSquare.size.y) {

			if (j < this->projectionDrawingSquare.size.y / 2) {
				rectHeight = this->projectionDrawingSquare.size.y - (2 * j);
			}
			else {
				rectHeight = this->projectionDrawingSquare.size.y - (2 * (this->projectionDrawingSquare.size.y - j));
			}

			float distance = std::min((this->getHeight(Obstacle)) / rectHeight, 100.0f);
			wallHeight = (this->getHeight(Obstacle) + cameraVerticalOffset) / distance;

			offset = (wallHeight - rectHeight) / 2;

			grey = this->map(pow(distance, 0.5), -10, pow(100, 0.5), 200, 0);

			//grey = this->map(pow(abs(this->projectionDrawingSquare.size.y / 2 - j * CHUNK_SIZE), 0.8), 0, pow(this->projectionDrawingSquare.size.y / 2, 0.8), 0, 180);

			this->drawRect(Coordinates{ p.x,(float)j + int(offset) }, Coordinates{ rectWidth, float(CHUNK_SIZE + int(offset)) }, { int(grey),int(grey),int(grey) });

			j += CHUNK_SIZE;
			
		}

		//if (distances[i].distance == distances[i].maxLength) continue;

		
		for (int k = 0; k < info[i].size(); k++) {
			rectHeight = this->getHeight(info[i][k].type) / info[i][k].distance;
			wallHeight =  (this->getHeight(Obstacle) + cameraVerticalOffset) / info[i][k].distance;
			offset = (wallHeight - rectHeight) / 2;


			grey = this->map(pow(info[i][k].distance, 0.5), 0, pow(100, .5), 1, 0);

			p.y = (this->projectionDrawingSquare.size.y - rectHeight) / 2 + offset;

			int c = int(info[i][k].colOffset) % IMAGE_WIDTH;

			rectHeight = rectHeight / WALL_PIXEL_HEIGHT;

			for (int j = 0; j < WALL_PIXEL_HEIGHT; j++) {
				
				RGBA color =(this->getTexture(info[i][k].type))->texture[(c % IMAGE_WIDTH)*IMAGE_HEIGHT + j % IMAGE_HEIGHT];

				if (color.a == 0) continue;

				color.r = float(color.r) * grey;
				color.g = float(color.g) * grey;
				color.b = float(color.b) * grey;
				this->drawRect(p, Coordinates{ rectWidth , rectHeight }, color.toRGB());

				p.y += rectHeight;
			}
		}	
	}
}