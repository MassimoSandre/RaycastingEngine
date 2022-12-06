#pragma once
#include <iostream>
#include <vector>
#include <cmath>

typedef struct {
	int r;
	int g;
	int b;
} RGB;

typedef struct RGBA{
	int r;
	int g;
	int b;
	int a;

	RGB toRGB() {
		return RGB{ r,g,b };
	}
} RGBA;

typedef struct Coordinates{
	float x;
	float y;

	float distance(Coordinates coord) {
		return sqrt((x - coord.x) * (x - coord.x) + (y - coord.y) * (y - coord.y));
	}
	double getAngle(Coordinates coord) {
		float length = distance(coord);
		if (length == 0) return 0;
		double cx = (double)(coord.x - x)/length;
		double sx = -(double)(coord.y - y)/length;
		return atan2(sx, cx);
	}
	Coordinates toInt() {
		return { float(int(x)), float(int(y)) };
	}
	Coordinates add(Coordinates coord) {
		return { x + coord.x, y + coord.y };
	}
} Coordinates;


typedef struct Size {
	int x;
	int y;

	Coordinates toCoordinates() {
		return Coordinates{ (float)x, (float)y };
	}
}Size;

typedef Size Cell;

typedef struct {
	Coordinates p1;
	Coordinates p2;
} Line;

//class QueueElement {
//public:
//	long long distance;
//	Cell cell;
//	Cell father;
//};

typedef struct {
	int east;
	int south;
} CellWeight;

enum RayType {Generic, Obstacle, View, EntitySegment};

typedef struct {
	Coordinates intersection;
	float colOffset;
} IntersectionInfo;

typedef struct {
	RayType type;
	float distance;
	float colOffset;
} RenderInfo;

typedef std::vector<RenderInfo> RayInfo;
typedef std::vector<RayInfo> RenderingInfo;

typedef struct {
	Coordinates topLeft;
	Coordinates size;
} Rect;