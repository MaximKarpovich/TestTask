#include "helm.h"

void Helm::set_item_info(const byte* thing, const size_t size) {
	memcpy((void*)&(this->helm_data_), (void*)&thing[1], sizeof(HelmData));
	this->type_ = (evil::RareType)thing[0];
}
std::string Helm::get_description() {
	return "Helm"; //can be any string containing object data.
}

value Helm::get_cost() {
	return this->helm_data_.cost;
}