#pragma once

#include "utils.cpp"
#include <string>

class Texture {
public:
	Size size;
	std::vector<RGBA> texture;

	Texture();
	Texture(std::string file);
	~Texture();
};