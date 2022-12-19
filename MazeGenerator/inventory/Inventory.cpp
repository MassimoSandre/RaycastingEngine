#include "Inventory.h"

Inventory::Inventory() {
	this->stacks.clear();
}

void Inventory::addItemStack(ItemStack itemstack) {
	for (int i = 0; i < this->stacks.size(); i++) {
		if (this->stacks[i].getItem()->id == itemstack.getItem()->id) {
			this->stacks[i].setAmount(this->stacks[i].getAmount() + itemstack.getAmount());
			return;
		}
	}
	this->stacks.push_back(itemstack);
}
