#pragma once

#include "MapElement.h"
#include "ElementProperties.h"
#include "ElementState.h"
#include <string>

class Wall : public MapElement{
public:
	Wall() ;
	Wall(ElementProperties* properties, std::string textureFileName);

	ElementState getDefaultState() override;
};