#include "jug.h"

void Jug::set_item_info(const byte* thing, const size_t size) {
	memcpy((void*)&(this->jug_data_), (void*)&thing[1], sizeof(JugData));
	this->type_ = (evil::RareType)thing[0];
}

std::string Jug::get_description() {
	return  "Jug"; //can be any string containing object data.
}

value Jug::get_cost() {
	return this->jug_data_.cost;
}