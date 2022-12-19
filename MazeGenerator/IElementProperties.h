#pragma once
class IElementProperties {
public:
	double height;
	double verticalOffset;

	IElementProperties* setHeight(double height);
	IElementProperties* setVerticalOffset(double verticalOffset);
};

