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
} RenderInfo;

typedef int RenderDIO;

typedef std::vector<RenderInfo> RayInfo;
typedef std::vector<RayInfo> ViewInfo;