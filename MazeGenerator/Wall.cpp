#include "Wall.h"

Wall::Wall() {
	this->type = Obstacle;
	this->textureId = 0;
	this->height = 8000;
}
Wall::Wall(Coordinates p1, Coordinates p2) : Segment(p1, p2) {
	this->type = Obstacle;
	this->textureId = 0;
	this->height = 8000;
}
Wall::Wall(Coordinates p1, double length, double angle) {
	this->type = Obstacle;
	this->textureId = 0;
	this->height = 8000;
}