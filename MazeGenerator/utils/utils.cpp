#pragma once
#include <iostream>
#include <vector>
#include <cmath>

typedef struct RGB {
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
	double x;
	double y;

	double distance(Coordinates coord) {
		if (coord.x == x) {
			return abs(coord.y - y);
		}
		if (coord.y == y) {
			return abs(coord.x - x);
		}
		return sqrt((x - coord.x) * (x - coord.x) + (y - coord.y) * (y - coord.y));
	}
	double getAngle(Coordinates coord) {
		double length = distance(coord);
		if (length == 0) return 0;
		double cx = (double)(coord.x - x)/length;
		double sx = -(double)(coord.y - y)/length;
		return atan2(sx, cx);
	}
	Coordinates toInt() {
		return { double(int(x)), double(int(y)) };
	}
	Coordinates add(Coordinates coord) {
		return { x + coord.x, y + coord.y };
	}

	Coordinates operator + (Coordinates const& coord) {
		return { x + coord.x, y + coord.y };
	}
	Coordinates operator - (Coordinates const& coord) {
		return { x - coord.x, y - coord.y };
	}
} Coordinates;


typedef struct Size {
	int x;
	int y;

	Coordinates toCoordinates() {
		return Coordinates{ (double)x, (double)y };
	}
}Size;

typedef Size Cell;

typedef struct Line {
	Coordinates p1;
	Coordinates p2;
} Line;

typedef struct CellWeight {
	int east;
	int south;
} CellWeight;

typedef struct IntersectionInfo {
	Coordinates intersection;
	double colOffset;
} IntersectionInfo;

typedef struct Rect {
	Coordinates topLeft;
	Coordinates size;
} Rect;

typedef struct Canvas {
	Rect drawingRect;
	Coordinates realSize;
} Canvas;

