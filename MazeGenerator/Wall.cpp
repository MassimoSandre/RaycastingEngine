#include "Wall.h"

Wall::Wall() {
	this->type = Obstacle;
}
Wall::Wall(Coordinates p1, Coordinates p2) : Segment(p1, p2) {
	this->type = Obstacle;
}
Wall::Wall(Coordinates p1, float length, double angle) {
	this->type = Obstacle;
}