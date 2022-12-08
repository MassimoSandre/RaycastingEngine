#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "utils.cpp"
#include <vector>
#include <memory>
#include "Segment.h"
#include "Player.h"
#include "Entity.h"
#include "Texture.h"

class Renderer {
private:
	GLFWwindow* window;	
	std::string windowTitle;

	/*
	Rect mazeDrawingSquare;
	Rect projectionDrawingSquare;
	*/
	std::vector<Canvas> drawingCanvas;

	Texture wallTexture;
	Texture entityTexture;

	float scaleX(float x, int canvas);
	float scaleY(float y, int canvas);
	float map(float value, float istart, float istop, float ostart, float ostop);
	Texture* getTexture(RayType type);
	float getHeight(RayType type);
public:
	Renderer(std::string windowTitle, Rect mazeDrawingSquare, Rect projectionDrawingSquare);
	~Renderer();

	void setMazeSize(Coordinates mazeRealSize);

	void init();

	Coordinates getMousePosition();
	void setMousePosition(Coordinates position);

	bool isKeyPressed(int key);
	bool leftClick();
	bool closing();

	void update();

	void drawPixel(int x, int y, int canvas=0);
	void drawPixel(int x, int y, RGB color, int canvas = 0);
	void drawPixel(Coordinates p, int canvas = 0);
	void drawPixel(Coordinates p, RGB color, int canvas = 0);

	void drawLine(int x1, int y1, int x2, int y2, int canvas = 0);
	void drawLine(int x1, int y1, int x2, int y2, RGB color, int canvas = 0);
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

	void drawCollectible(std::shared_ptr<Entity>& collectible, int canvas = 0);
	void drawCollectibles(std::vector<std::shared_ptr<Entity>>& collectibles, int canvas = 0);
	void drawCollectible(std::shared_ptr<Entity>& collectible, RGB color, int canvas = 0);
	void drawCollectibles(std::vector<std::shared_ptr<Entity>>& collectibles, RGB color, int canvas = 0);

	void drawView(std::vector<std::shared_ptr<Segment>>& r, int canvas = 0, bool connect = true);
	void drawView(std::vector<std::shared_ptr<Segment>>& r, RGB color, int canvas = 0,bool connect = true);

	void drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, int canvas = 0);
	void drawTriangle(Coordinates p1, Coordinates p2, Coordinates p3, RGB color, int canvas = 0);

	void drawRect(Coordinates topLeft, Coordinates size, int canvas = 0);
	void drawRect(Coordinates topLeft, Coordinates size, RGB color, int canvas = 0);
	void drawRect(Rect rect, int canvas = 0);
	void drawRect(Rect rect, RGB color, int canvas = 0);

	void drawQuadrilateral(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p4, int canvas);
	void drawQuadrilateral(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p4, RGB color, int canvas);

	void drawCircle(Coordinates center, float radius, int canvas = 0);
	void drawCircle(Coordinates center, float radius, RGB color, int canvas = 0);

	void drawProjection(RenderingInfo info, float cameraVerticalOffset, int canvas = 0);
};