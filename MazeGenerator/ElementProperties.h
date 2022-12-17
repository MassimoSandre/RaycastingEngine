#pragma once
class ElementProperties {
public:
	double height;
	double verticalOffset;

	static ElementProperties* getDefault();

	ElementProperties* setHeight(double height);
	ElementProperties* setVerticalOffset(double verticalOffset);

};

