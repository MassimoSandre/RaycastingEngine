#pragma once
class ObstacleProperties {
public:
	double height;
	double verticalOffset;

	static ObstacleProperties* getDefault();

	ObstacleProperties* setHeight(double height);
	ObstacleProperties* setVerticalOffset(double verticalOffset);
};

