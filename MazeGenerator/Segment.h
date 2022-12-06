#pragma once
#include "utils.cpp"
#include <iostream>
#include <memory>
class Segment {
public:
	RayType type;
	Coordinates p1;
	Coordinates p2;
	float length;
	double angle;


	void calculateP2();

	Segment();
	Segment(Coordinates p1, Coordinates p2);
	Segment(Coordinates p1, float length, double angle);

	void changePoints(Coordinates p1, Coordinates p2);
	void changeP1(Coordinates p1);
	void changeP2(Coordinates p2);

	void add(Segment s);

	void move(Coordinates p1);

	void setLength(float length);
	void changeLength(float delta);
	void setAngle(double angle);
	void rotate(double angle);
	void translate(Coordinates offsets);

	void faceTo(Coordinates p);

	IntersectionInfo getIntersection(std::shared_ptr<Segment> segment);
};