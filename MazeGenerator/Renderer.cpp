#include "Renderer.h"
#include "Entity.h"

#define TRIANGLES_IN_CIRCLE 6

float Renderer::scaleX(float x, int canvas) {
	float result = x;
	result = result * (this->drawingCanvas[canvas].drawingRect.size.x / this->drawingCanvas[canvas].realSize.x);
	result += this->drawingCanvas[canvas].drawingRect.topLeft.x;
	result = (float)2 * result / (this->drawingCanvas[0].drawingRect.size.x) - 1;
	return result;
}
float Renderer::scaleY(float y, int canvas) {
	float result = y;
	result = result * (this->drawingCanvas[canvas].drawingRect.size.y / this->drawingCanvas[canvas].realSize.y);
	result += this->drawingCanvas[canvas].drawingRect.topLeft.y;
	result = (float)2 * result / (this->drawingCanvas[0].drawingRect.size.y) - 1;
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


Renderer::Renderer(std::string windowTitle, Rect mazeDrawingSquare, Rect projectionDrawingSquare) :
	wallTexture("textures/brickwall.texture"), 
	entityTexture("textures/collectible.texture") {
	
	this->drawingCanvas.resize(3);
	this->drawingCanvas[0] = { {mazeDrawingSquare.topLeft, {mazeDrawingSquare.size.x + projectionDrawingSquare.size.x, mazeDrawingSquare.size.y}},{mazeDrawingSquare.size.x + projectionDrawingSquare.size.x, mazeDrawingSquare.size.y} };

	this->windowTitle = windowTitle;
	
	this->drawingCanvas[1].drawingRect = mazeDrawingSquare;
	this->drawingCanvas[1].realSize = mazeDrawingSquare.size;
	
	this->drawingCanvas[2].drawingRect = projectionDrawingSquare;
	this->drawingCanvas[2].realSize = projectionDrawingSquare.size;

	this->init();
}

void Renderer::setMazeSize(Coordinates mazeRealSize) {
	this->drawingCanvas[1].realSize = mazeRealSize;
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

void Renderer::drawPixel(int x, int y, int canvas) {
	this->drawPixel(x, y, { 255,255,255 }, canvas);
}
void Renderer::drawPixel(int x, int y, RGB color, int canvas) {
	float xf = this->scaleX(x, canvas);
	float yf = this->scaleY(y, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2f(xf, -yf);
	glEnd();
}
void Renderer::drawPixel(Coordinates p, int canvas) {
	this->drawPixel(p.x, p.y, { 255,255,255 }, canvas);
}
void Renderer::drawPixel(Coordinates p, RGB color, int canvas) {
	this->drawPixel(p.x, p.y, color, canvas);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, int canvas) {
	this->drawLine(x1, y1, x2, y2, { 255,255,255 }, canvas);
}
void Renderer::drawLine(int x1, int y1, int x2, int y2, RGB color, int canvas) {
	float xf1 = this->scaleX(x1, canvas);
	float yf1 = this->scaleY(y1, canvas);

	float xf2 = this->scaleX(x2, canvas);
	float yf2 = this->scaleY(y2, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(xf1, -yf1);
	glVertex2f(xf2, -yf2);
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
	for (Line l : lines) {
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

void Renderer::drawCollectible(std::shared_ptr<Entity>& collectible, int canvas) {
	this->drawCollectible(collectible, { 255,255,255 }, canvas);
}
void Renderer::drawCollectibles(std::vector<std::shared_ptr<Entity>>& collectibles, int canvas) {
	this->drawCollectibles(collectibles, { 255,255,255 }, canvas);
}
void Renderer::drawCollectible(std::shared_ptr<Entity>& collectible, RGB color, int canvas) {
	this->drawCircle(collectible->center, collectible->length / 2, color, canvas);
}
void Renderer::drawCollectibles(std::vector<std::shared_ptr<Entity>>& collectibles, RGB color, int canvas) {
	for (int i = 0; i < collectibles.size(); i++) {
		this->drawCircle(collectibles[i]->center, collectibles[i]->length / 2, color, canvas);
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

void Renderer::drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, int canvas) {
	this->drawTriangle(p1, p2, p3, { 255,255,255 }, canvas);
}
void Renderer::drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, RGB color, int canvas) {
	float xf1 = this->scaleX(p1.x, canvas);
	float yf1 = this->scaleY(p1.y, canvas);

	float xf2 = this->scaleX(p2.x, canvas);
	float yf2 = this->scaleY(p2.y, canvas);

	float xf3 = this->scaleX(p3.x, canvas);
	float yf3 = this->scaleY(p3.y, canvas);

	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_TRIANGLES);
	glVertex2f(xf1, -yf1);
	glVertex2f(xf2, -yf2);
	glVertex2f(xf3, -yf3);
	glEnd();
}

void Renderer::drawRect(Coordinates topLeft, Coordinates size, int canvas) {
	this->drawRect(topLeft, size, { 255,255,255 }, canvas);

}
void Renderer::drawRect(Coordinates topLeft, Coordinates size, RGB color, int canvas) {
	float x1 = topLeft.x,
		x2 = topLeft.x + size.x;
	float y1 = topLeft.y,
		y2 = topLeft.y + size.y;

	float x1f = this->scaleX(x1, canvas);
	float y1f = this->scaleY(y1, canvas);

	float x2f = this->scaleX(x2, canvas);
	float y2f = this->scaleY(y2, canvas);

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
void Renderer::drawRect(Rect rect, int canvas) {
	this->drawRect(rect.topLeft, rect.size, { 255,255,255 }, canvas);
}
void Renderer::drawRect(Rect rect, RGB color, int canvas) {
	this->drawRect(rect.topLeft, rect.size, color, canvas);
}

void Renderer::drawCircle(Coordinates center, float radius, int canvas) {
	this->drawCircle(center, radius, { 255,255,255 }, canvas);
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
	glVertex2f(p1.x, -p1.y);
	glVertex2f(p2.x, -p2.y);
	glVertex2f(p3.x, -p3.y);
	glVertex2f(p3.x, -p3.y);
	glVertex2f(p4.x, -p4.y);
	glVertex2f(p2.x, -p2.y);
	glEnd();
}

void Renderer::drawCircle(Coordinates center, float radius, RGB color, int canvas) {
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

void Renderer::drawProjection(RenderingInfo info, float cameraVerticalOffset, int canvas) {
	int len = info.size();

	const int CHUNK_SIZE = 4;
	const int WALL_PIXEL_HEIGHT = 32;
	const int IMAGE_HEIGHT = 16;
	const int IMAGE_WIDTH = 16;


	float rectWidth = this->drawingCanvas[canvas].realSize.x / len;

	float rectHeight, wallHeight;
	float offset;
	
	float grey;
	Coordinates p;


	for (int i = 0; i < len; i++) {
		
		p.x =  rectWidth * i;

		float d = this->getHeight(Obstacle) / this->drawingCanvas[canvas].realSize.y;
		
		wallHeight = (this->getHeight(Obstacle) + cameraVerticalOffset) / d;
		
		int j = - (wallHeight - this->drawingCanvas[canvas].realSize.y)*2;
		while (j < this->drawingCanvas[canvas].realSize.y) {

			if (j < this->drawingCanvas[canvas].realSize.y / 2) {
				rectHeight = this->drawingCanvas[canvas].realSize.y - (2 * j);
			}
			else {
				rectHeight = this->drawingCanvas[canvas].realSize.y - (2 * (this->drawingCanvas[canvas].realSize.y - j));
			}

			float distance = std::min((this->getHeight(Obstacle)) / rectHeight, 100.0f);
			wallHeight = (this->getHeight(Obstacle) + cameraVerticalOffset) / distance;

			offset = (wallHeight - rectHeight) / 2;

			grey = this->map(pow(distance, 0.5), -10, pow(100, 0.5), 200, 0);

			//grey = this->map(pow(abs(this->projectionDrawingSquare.size.y / 2 - j * CHUNK_SIZE), 0.8), 0, pow(this->projectionDrawingSquare.size.y / 2, 0.8), 0, 180);

			this->drawRect(Coordinates{ p.x,(float)j + int(offset) }, Coordinates{ rectWidth, float(CHUNK_SIZE + int(offset)) }, { int(grey),int(grey),int(grey) }, canvas);

			j += CHUNK_SIZE;
			
		}

		//if (distances[i].distance == distances[i].maxLength) continue;

		
		for (int k = 0; k < info[i].size(); k++) {
			rectHeight = this->getHeight(info[i][k].type) / info[i][k].distance;
			wallHeight =  (this->getHeight(Obstacle) + cameraVerticalOffset) / info[i][k].distance;
			offset = (wallHeight - rectHeight) / 2;


			grey = this->map(pow(info[i][k].distance, 0.5), 0, pow(100, .5), 1, 0);

			p.y = (this->drawingCanvas[canvas].realSize.y - rectHeight) / 2 + offset;

			int c = int(info[i][k].colOffset + (info[i][k].type == EntitySegment ? 2.0 : 0)) % IMAGE_WIDTH;

			rectHeight = rectHeight / (WALL_PIXEL_HEIGHT * (this->getHeight(info[i][k].type) / this->getHeight(Obstacle) ));

			for (int j = 0; j < (WALL_PIXEL_HEIGHT * (this->getHeight(info[i][k].type) / this->getHeight(Obstacle))); j++) {
				RGBA color =(this->getTexture(info[i][k].type))->texture[(c % IMAGE_WIDTH)*IMAGE_HEIGHT + j % IMAGE_HEIGHT];

				if (color.a != 0) {
					color.r = float(color.r) * grey;
					color.g = float(color.g) * grey;
					color.b = float(color.b) * grey;
					this->drawRect(p, Coordinates{ rectWidth , rectHeight }, color.toRGB(), canvas);
				}

				p.y += rectHeight;
			}
		}	
	

	}

	

}