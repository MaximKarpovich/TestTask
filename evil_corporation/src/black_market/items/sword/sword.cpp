#include "sword.h"

void Sword::set_item_info(const byte* thing, const size_t size) {
	memcpy((void*)&(this->sword_data_), (void*)&thing[1], sizeof(SwordData));
	this->type_ = (evil::RareType)thing[0];
}

std::string Sword::get_description() {
	return  "Sword"; //can be any string containing object data.
}

value Sword::get_cost() {
	return this->sword_data_.cost;
}