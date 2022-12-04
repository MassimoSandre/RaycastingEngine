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

	RGB wallTexture[16][16] = { {RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{198,104,81},RGB{124,69,54},RGB{124,69,54},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{155,86,67},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125}},{RGB{177,98,77},RGB{155,86,67},RGB{115,63,49},RGB{139,110,103},RGB{198,104,81},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{155,86,67},RGB{143,80,63},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{155,86,67},RGB{139,110,103},RGB{155,86,67},RGB{143,80,63},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{139,110,103},RGB{162,134,125},RGB{169,148,141},RGB{169,148,141},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{162,134,125},RGB{162,134,125},RGB{162,134,125},RGB{169,148,141},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{198,104,81},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125},RGB{155,86,67},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{198,104,81},RGB{143,80,63},RGB{124,69,54},RGB{162,134,125},RGB{155,86,67},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{198,104,81},RGB{143,80,63},RGB{115,63,49},RGB{139,110,103},RGB{177,98,77},RGB{143,80,63},RGB{124,69,54},RGB{162,134,125},RGB{198,104,81},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{198,104,81},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{155,86,67},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125},RGB{198,104,81},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{155,86,67},RGB{143,80,63},RGB{115,63,49},RGB{162,134,125}},{RGB{177,98,77},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{162,134,125},RGB{162,134,125},RGB{169,148,141},RGB{169,148,141},RGB{177,98,77},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{139,110,103},RGB{162,134,125},RGB{169,148,141},RGB{169,148,141}},{RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{198,104,81},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125},RGB{155,86,67},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{198,104,81},RGB{143,80,63},RGB{115,63,49},RGB{162,134,125}},{RGB{177,98,77},RGB{155,86,67},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{143,80,63},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{162,134,125}},{RGB{155,86,67},RGB{143,80,63},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{143,80,63},RGB{124,69,54},RGB{139,110,103},RGB{155,86,67},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{162,134,125},RGB{162,134,125},RGB{162,134,125},RGB{169,148,141},RGB{177,98,77},RGB{143,80,63},RGB{143,80,63},RGB{139,110,103},RGB{162,134,125},RGB{162,134,125},RGB{169,148,141},RGB{169,148,141},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{198,104,81},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125},RGB{155,86,67},RGB{143,80,63},RGB{143,80,63},RGB{139,110,103},RGB{198,104,81},RGB{143,80,63},RGB{124,69,54},RGB{162,134,125},RGB{155,86,67},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103}},{RGB{177,98,77},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{177,98,77},RGB{143,80,63},RGB{124,69,54},RGB{139,110,103},RGB{198,104,81},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{162,134,125}},{RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{155,86,67},RGB{124,69,54},RGB{115,63,49},RGB{162,134,125},RGB{177,98,77},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{155,86,67},RGB{124,69,54},RGB{115,63,49},RGB{169,148,141}},{RGB{177,98,77},RGB{155,86,67},RGB{124,69,54},RGB{139,110,103},RGB{139,110,103},RGB{162,134,125},RGB{162,134,125},RGB{169,148,141},RGB{177,98,77},RGB{155,86,67},RGB{143,80,63},RGB{139,110,103},RGB{162,134,125},RGB{162,134,125},RGB{169,148,141},RGB{169,148,141}} };

	void init();
	float map(float value, float istart, float istop, float ostart, float ostop);
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

	void drawRect(Coordinates topLeft, Coordinates size);
	void drawRect(Coordinates topLeft, Coordinates size, RGB color);

	void drawProjection(std::vector<RenderInfo> distances);

};