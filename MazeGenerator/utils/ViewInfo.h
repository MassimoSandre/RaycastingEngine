#pragma once

#include <memory>
#include <vector>
#include "../obstacle/obstacles/IObstacle.h"

typedef struct {
	double distance;
	double colOffset;
	double height;
	double verticalOffset;
	std::shared_ptr<IElement> element;
} v;

typedef std::vector<v> ri;
typedef std::vector<ri> ViewInfo;