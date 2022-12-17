#pragma once

#include <memory>
#include <vector>
#include "../MapElement.h"

typedef struct {
	double distance;
	double colOffset;
	double height;
	double verticalOffset;
	std::shared_ptr<MapElement> element;
} v;

typedef std::vector<v> ri;
typedef std::vector<ri> ViewInfo;