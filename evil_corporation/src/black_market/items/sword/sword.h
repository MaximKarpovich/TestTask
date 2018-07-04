#pragma once

#include "../base/base_item.h"

using SwordData = struct SwordDataStruct
{
	value cost;
	value damage;
	value height;
};

class Sword : public Item
{
public:
	Sword() {};
	~Sword() {};

	virtual void set_item_info(const byte* thing, const size_t size);
	virtual std::string get_description();
	virtual value get_cost();

private:
	SwordData sword_data_;
	evil::RareType type_;
};