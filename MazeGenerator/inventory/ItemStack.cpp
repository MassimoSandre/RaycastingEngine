#include "ItemStack.h"

void ItemStack::setItem(std::shared_ptr<IElement> element) {
	this->element = element;
}
void ItemStack::setAmount(int amount) {
	this->stackSize = amount;
}

std::shared_ptr<IElement>& ItemStack::getItem() {
	return this->element;
}
int ItemStack::getAmount() {
	return this->stackSize;
}