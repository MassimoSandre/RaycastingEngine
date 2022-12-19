#include "EntityProperties.h"

IElementProperties* EntityProperties::getDefault() {
	EntityProperties* e = new EntityProperties;
	e->height = 8000;
	e->verticalOffset = 0;
	return e;
}

//EntityProperties* EntityProperties::setHeight(double height) {
//	this->height = height;
//	return this;
//}
//EntityProperties* EntityProperties::setVerticalOffset(double verticalOffset) {
//	this->verticalOffset = verticalOffset;
//	return this;
//}
