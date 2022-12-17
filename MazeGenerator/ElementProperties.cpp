#include "ElementProperties.h"

ElementProperties* ElementProperties::getDefault() {
	ElementProperties* e = new ElementProperties;
	e->height = 8000;
	e->verticalOffset = 0;
	return e;
}

ElementProperties* ElementProperties::setHeight(double height) {
	this->height = height;
	return this;
}
ElementProperties* ElementProperties::setVerticalOffset(double verticalOffset) {
	this->verticalOffset = verticalOffset;
	return this;
}
