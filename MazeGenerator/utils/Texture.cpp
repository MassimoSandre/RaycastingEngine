#include "Texture.h"
#include <fstream>

Texture::Texture() {}

Texture::Texture(std::string file) {
	if (file == "") return;
	std::ifstream rf(file, std::ios::out | std::ios::binary);

	if (!rf) {
		throw std::ios_base::failure("file not found");
	}
	
	uint8_t input;

	rf.read((char*)&input, 1);
	this->size.x = input;
	rf.read((char*)&input, 1);
	this->size.y = input;

	this->texture.resize(this->size.x*this->size.y);
	
	for (int i = 0; i < this->size.x; i++) {
		for (int j = 0; j < this->size.y; j++) {
			rf.read((char*)&input, 1);
			this->texture[i*this->size.y + j].r = input;
			rf.read((char*)&input, 1);
			this->texture[i * this->size.y + j].g = input;
			rf.read((char*)&input, 1);
			this->texture[i * this->size.y + j].b = input;
			rf.read((char*)&input, 1);
			this->texture[i * this->size.y + j].a = input;
		}
	}

	rf.close();
	if (!rf.good()) {
		throw std::ios_base::failure("error while reading file");
	}
}

Texture::~Texture() {}