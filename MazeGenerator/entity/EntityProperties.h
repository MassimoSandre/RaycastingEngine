#pragma once
class EntityProperties {
public:
	double height;
	double verticalOffset;

	static EntityProperties* getDefault();

	EntityProperties* setHeight(double height);
	EntityProperties* setVerticalOffset(double verticalOffset);
};

