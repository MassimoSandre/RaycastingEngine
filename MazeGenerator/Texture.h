#pragma once
#include "utils.cpp"
#include <iostream>
#include <fstream>

class Texture {
public:
	Size size;
	std::vector<RGBA> texture;

	Texture(std::string file);
	~Texture();
};