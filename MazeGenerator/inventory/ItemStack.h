#pragma once

#include <memory>
#include "../IElement.h"

class ItemStack {
private:
	std::shared_ptr<IElement> element;
	int stackSize;
public:
	ItemStack() : element(nullptr), stackSize(0) {}
	ItemStack(std::shared_ptr<IElement> element) : element(element), stackSize(0) {}
	ItemStack(std::shared_ptr<IElement> element, int amount) : element(element), stackSize(amount) {}

	void setItem(std::shared_ptr<IElement> element);
	void setAmount(int amount);

	std::shared_ptr<IElement>& getItem();
	int getAmount();
};

