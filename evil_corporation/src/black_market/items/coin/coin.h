#pragma once

#include "../base/base_item.h"

using CoinData = struct CoinDataStruct
{
	value cost;
};

class Coin : public Item
{
public:
	Coin() {};
	~Coin() {};

	virtual void set_item_info(const byte* thing, const size_t size);
	virtual std::string get_description();
	virtual value get_cost();

private:
	CoinData coin_data_;
	evil::RareType type_;
};