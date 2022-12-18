#pragma once
#include "utils/Texture.h"

class IElement {
protected:
	double HEIGHT;
	double VERTICAL_OFFSET;

public:
	Texture texture;

	IElement() {}
	IElement(std::string textureFileName) : texture(textureFileName) {}
};

