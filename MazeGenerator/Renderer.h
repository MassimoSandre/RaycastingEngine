#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "utils/utils.cpp"
#include "utils/Segment.h"

#define TRIANGLES_IN_CIRCLE 20

class Renderer {
private:
	GLFWwindow* window;	
	std::string windowTitle;

	std::vector<Canvas> drawingCanvas;

	double scaleX(double x, int canvas);
	double scaleY(double y, int canvas);
public:
	Renderer(Size windowSize, std::string windowTitle="Window");
	~Renderer();

	void addCanvas(Canvas canvas);

	void setCanvasRealSize(int canvasIndex, Coordinates realSize);

	void init();

	Coordinates getMousePosition();
	void setMousePosition(Coordinates position);

	bool isKeyPressed(int key);
	bool isKeyReleased(int key);
	bool leftClick();
	bool closing();
	
	void hideCursor();
	void showCursor();

	void update();

	void drawPixel(double x, double y, int canvas=0);
	void drawPixel(double x, double y, RGB color, int canvas = 0);
	void drawPixel(Coordinates p, int canvas = 0);
	void drawPixel(Coordinates p, RGB color, int canvas = 0);

	void drawLine(double x1, double y1, double x2, double y2, int canvas = 0);
	void drawLine(double x1, double y1, double x2, double y2, RGB color, int canvas = 0);
	void drawLine(Coordinates p1, Coordinates p2, int canvas = 0);
	void drawLine(Coordinates p1, Coordinates p2, RGB color, int canvas = 0);
	void drawLine(Line l, int canvas = 0);
	void drawLine(Line l, RGB color, int canvas = 0);
	
	void drawLines(std::vector<Line>& lines, int canvas = 0);
	void drawLines(std::vector<Line>& lines, RGB color, int canvas = 0);
	
	void drawSegment(std::shared_ptr<Segment>& segment, int canvas = 0);
	void drawSegments(std::vector<std::shared_ptr<Segment>>& segments, int canvas = 0);
	void drawSegment(std::shared_ptr<Segment>& segment, RGB color, int canvas = 0);
	void drawSegments(std::vector<std::shared_ptr<Segment>>& segments, RGB color, int canvas = 0);

	/*void drawCollectible(std::shared_ptr<Entity>& collectible, int canvas = 0);
	void drawCollectibles(std::vector<std::shared_ptr<Entity>>& collectibles, int canvas = 0);
	void drawCollectible(std::shared_ptr<Entity>& collectible, RGB color, int canvas = 0);
	void drawCollectibles(std::vector<std::shared_ptr<Entity>>& collectibles, RGB color, int canvas = 0);*/

	void drawView(std::vector<std::shared_ptr<Segment>>& r, int canvas = 0, bool connect = true);
	void drawView(std::vector<std::shared_ptr<Segment>>& r, RGB color, int canvas = 0,bool connect = true);
	void drawView(std::vector<std::shared_ptr<Segment>>& r, RGB color, Coordinates center, int canvas = 0, bool connect = true);

	void drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, int canvas = 0);
	void drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, RGB color, int canvas = 0);

	void drawRect(Coordinates topLeft, Coordinates size, int canvas = 0);
	void drawRect(Coordinates topLeft, Coordinates size, RGB color, int canvas = 0);
	void drawRect(Rect rect, int canvas = 0);
	void drawRect(Rect rect, RGB color, int canvas = 0);

	void drawQuadrilateral(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p4, int canvas);
	void drawQuadrilateral(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p4, RGB color, int canvas);

	void drawCircle(Coordinates center, double radius, int canvas = 0);
	void drawCircle(Coordinates center, double radius, RGB color, int canvas = 0);
};