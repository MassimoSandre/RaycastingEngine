#pragma once
#include "../IElementProperties.h"

class EntityProperties : public IElementProperties {
public:
	static IElementProperties* getDefault();
};

