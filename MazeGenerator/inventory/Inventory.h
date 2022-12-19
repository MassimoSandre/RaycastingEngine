#pragma once
#include <vector>
#include "ItemStack.h"

class Inventory {
public:
	std::vector<ItemStack> stacks;

	Inventory();

	void addItemStack(ItemStack itemstack);
};

