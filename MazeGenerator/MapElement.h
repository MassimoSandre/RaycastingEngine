#pragma once
#include "ElementProperties.h"
#include "Texture.h"

class ElementState;
class MapElement {
protected:
	double HEIGHT;
	double VERTICAL_OFFSET;

	std::shared_ptr<MapElement> self;

public:
	Texture texture;

	MapElement() {}
	MapElement(ElementProperties* properties, std::string textureFileName) : texture(textureFileName) {
		this->HEIGHT = properties->height;
		this->VERTICAL_OFFSET = properties->verticalOffset;
	}

	int a() { return 0; }

	virtual ElementState getDefaultState() = 0;

};