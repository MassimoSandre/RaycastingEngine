#include "Segment.h"
#include <iostream>
#include <cmath>

void Segment::calculateP2() {
	this->p2.x = (p1.x + this->length * cos(this->angle));
	this->p2.y = (p1.y - this->length * sin(this->angle));
}

Segment::Segment() {
	this->type = Generic;
	this->verticalOffset = 0.0;
	this->angle = 0.0;
	this->length = 0.0;
	this->p1 = { 0,0 };
	this->p2 = { 0,0 };
}

Segment::Segment(Coordinates p1, Coordinates p2) {
	this->type = Generic;
	this->verticalOffset = 0.0;
	this->p1 = p1;
	this->p2 = p2;
	this->length = p1.distance(p2);
	this->angle = p1.getAngle(p2);
}
Segment::Segment(Coordinates p1, double length, double angle) {
	this->type = Generic;
	this->verticalOffset = 0.0;
	this->p1 = p1;
	this->length = length;
	this->angle = angle;
	this->calculateP2();
}

void Segment::changePoints(Coordinates p1, Coordinates p2) {
	this->p1 = p1;
	this->p2 = p2;
	this->length = p1.distance(p2);
	this->angle = p1.getAngle(p2);
}
void Segment::changeP1(Coordinates p1) {
	this->p1 = p1;
	this->length = p1.distance(p2);
	this->angle = p1.getAngle(p2);
}
void Segment::changeP2(Coordinates p2) {
	this->p2 = p2;
	this->length = p1.distance(p2);
	this->angle = p1.getAngle(p2);
}

void Segment::add(Segment s) {
	this->changeP2({ this->p2.x + (s.p2.x - s.p1.x), this->p2.y + (s.p2.y - s.p1.y) });
}

void Segment::move(Coordinates p1) {
	this->p1 = p1;
	this->calculateP2();
}

void Segment::setLength(double length) {
	this->length = length;
	this->calculateP2();
}
void Segment::changeLength(double delta) {
	this->length+=delta;
	this->calculateP2();
}
void Segment::setAngle(double angle) {
	this->angle = angle;
	this->calculateP2();
}
void Segment::rotate(double angle) {
	this->angle += angle;
	this->calculateP2();
}
void Segment::translate(Coordinates offsets) {
	this->p1.x += offsets.x;
	this->p1.y += offsets.y;
	this->p2.x += offsets.x;
	this->p2.y += offsets.y;
}



IntersectionInfo Segment::getIntersection(Segment& segment) {

	double a1, b1, c1, a2, b2, c2, den;

	a1 = (this->p2.y) - (this->p1.y);
	b1 = (this->p1.x) - (this->p2.x);
	c1 = a1 * (this->p1.x) + b1 * (this->p1.y);

	a2 = (segment.p2.y) - (segment.p1.y);
	b2 = (segment.p1.x) - (segment.p2.x);
	c2 = a2 * (segment.p1.x) + b2 * (segment.p1.y);

	den = a1 * b2 - a2 * b1;

	if (den == 0.0) {
		return { -1,-1 };
	}

	IntersectionInfo intersection = { ((b2 * c1 - b1 * c2) / den), ((a1 * c2 - a2 * c1) / den), 0 };

	intersection.colOffset = segment.p1.distance(intersection.intersection);
	if (intersection.colOffset > segment.length) return { -1,-1 };
	if (segment.p2.distance(intersection.intersection) > segment.length) return { -1,-1 };
	intersection.colOffset -= segment.length/2.0;
	return intersection;
}