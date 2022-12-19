#include "IElementProperties.h"

IElementProperties* IElementProperties::setHeight(double height) {
	this->height = height;
	return this;
}
IElementProperties* IElementProperties::setVerticalOffset(double verticalOffset) {
	this->verticalOffset = verticalOffset;
	return this;
}
