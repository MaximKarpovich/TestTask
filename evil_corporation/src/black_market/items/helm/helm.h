#pragma once

#include "../base/base_item.h"

using HelmData = struct HelmDataStruct
{
	value cost;
	size  size;
	value armor;
};

class Helm : public Item
{
public:
	Helm() {};
	~Helm() {};

	virtual void set_item_info(const byte* thing, const size_t size);
	virtual std::string get_description();
	virtual value get_cost();

private:
	HelmData helm_data_;
	evil::RareType type_;
};