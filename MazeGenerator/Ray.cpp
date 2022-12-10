#include "Ray.h"

Ray::Ray() {
	this->type = View;
}
Ray::Ray(Coordinates p1, Coordinates p2) : Segment(p1,p2) {
	this->type = View;
}
Ray::Ray(Coordinates p1, double length, double angle) : Segment(p1, length, angle) {
	this->type = View;
}