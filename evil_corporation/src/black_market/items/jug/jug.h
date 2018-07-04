#pragma once

#include "../base/base_item.h"

using JugData = struct JugDataStruct
{
	value cost;
	size  R;
	size  H;
};

class Jug : public Item
{
public:
	Jug() {};
	~Jug() {};

	virtual void set_item_info(const byte* thing, const size_t size);
	virtual std::string get_description();
	virtual value get_cost();

private:
	JugData jug_data_;
	evil::RareType type_;
};