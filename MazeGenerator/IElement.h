#pragma once
#include "utils/Texture.h"

class IElement {
protected:
	double HEIGHT;
	double VERTICAL_OFFSET;

public:
	Texture texture;

	IElement() : HEIGHT(0), VERTICAL_OFFSET(0) {}
	IElement(std::string textureFileName) : HEIGHT(0), VERTICAL_OFFSET(0),texture(textureFileName) {}
};

