#include "coin.h"

void Coin::set_item_info(const byte* thing, const size_t size) {
	memcpy((void*)&(this->coin_data_), (void*)&thing[1], sizeof(CoinData));
	this->type_ = (evil::RareType)thing[0];
}
std::string Coin::get_description() {
	return  "Coin"; //can be any string containing object data.
}

value Coin::get_cost() {
	return this->coin_data_.cost;
}