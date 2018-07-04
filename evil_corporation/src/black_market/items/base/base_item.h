#pragma once

#include "external_api/api.h"

#include <string>
#include <memory>

/*Base class for all items*/
class Item {
public:
	Item() {};
	virtual ~Item() {};

	/*Construct object depending on it`s type*/
	virtual void set_item_info(const byte* thing, const size_t size) = 0;
	/*Return description depending on type*/
	virtual std::string get_description() = 0;
	virtual value get_cost() = 0;
};

using ItemPtr = std::shared_ptr<Item>;