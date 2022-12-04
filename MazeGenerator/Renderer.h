#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "utils.cpp"
#include <vector>
#include <memory>
#include "Segment.h"
#include "Raycaster.h"

class Renderer {
private:
	GLFWwindow* window;
	std::shared_ptr<Raycaster> caster;
	std::vector<std::shared_ptr<Segment>> walls;

	int screenWidth;
	int screenHeight;
	std::string windowTitle;

	void init();
public:
	Renderer();
	Renderer(int screenWidth, int screenHeight);
	Renderer(int screenWidth, int screenHeight, std::string windowTitle);
	~Renderer();

	Coordinates getMousePosition();

	bool update();

	void addWall(std::shared_ptr<Segment> segment);
	void addWalls(std::vector<std::shared_ptr<Segment>> segments);

	void drawPixel(int x, int y);
	void drawPixel(int x, int y, RGB color);
	void drawPixel(Coordinates p);
	void drawPixel(Coordinates p, RGB color);

	void drawLine(int x1, int y1, int x2, int y2);
	void drawLine(int x1, int y1, int x2, int y2, RGB color);
	void drawLine(Coordinates p1, Coordinates p2);
	void drawLine(Coordinates p1, Coordinates p2, RGB color);
	void drawLine(Line l);
	void drawLine(Line l, RGB color);
	
	void drawLines(std::vector<Line> lines);
	void drawLines(std::vector<Line> lines, RGB color);
	
	void drawSegment(std::shared_ptr<Segment> segment);
	void drawSegments(std::vector<std::shared_ptr<Segment>> segments);
	void drawSegment(std::shared_ptr<Segment> segment, RGB color);
	void drawSegments(std::vector<std::shared_ptr<Segment>> segments, RGB color);

	void drawView(std::shared_ptr<Raycaster> r, bool connect = true);
	void drawView(std::shared_ptr<Raycaster> r, RGB color, bool connect = true);

	void drawRect(Coordinates topLeft, Size size);
	void drawRect(Coordinates topLeft, Size size, RGB color);

	void drawProjection(std::vector<Coordinates> distances);
};