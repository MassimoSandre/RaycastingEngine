#include "ObstacleProperties.h"

IElementProperties* ObstacleProperties::getDefault() {
	ObstacleProperties* e = new ObstacleProperties;
	e->height = 8000;
	e->verticalOffset = 0;
	return e;
}

//ObstacleProperties* ObstacleProperties::setHeight(double height) {
//	this->height = height;
//	return this;
//}
//ObstacleProperties* ObstacleProperties::setVerticalOffset(double verticalOffset) {
//	this->verticalOffset = verticalOffset;
//	return this;
//}
