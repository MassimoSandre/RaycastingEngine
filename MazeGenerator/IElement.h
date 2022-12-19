#pragma once
#include "utils/Texture.h"
#include "Register.h"

class IElement {
protected:
	double HEIGHT;
	double VERTICAL_OFFSET;
public:
	int id;

	Texture texture;

	IElement() : HEIGHT(0), VERTICAL_OFFSET(0) { this->id = Register::getNewElementId(); }
	IElement(std::string textureFileName) : HEIGHT(0), VERTICAL_OFFSET(0), texture(textureFileName) { this->id = Register::getNewElementId(); }
};

