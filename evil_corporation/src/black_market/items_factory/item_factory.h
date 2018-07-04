#pragma once

#include "external_api/api.h"

#include "../items/base/base_item.h"

#include <map>
#include <memory>

/*Templated factory for creating item. First all items
  must be registered. After objects can be created*/
template <class BaseType>
class Creator
{
public:
	virtual ~Creator() {}
	virtual BaseType* create() = 0;
};

template <class DerivedType, class BaseType>
class DerivedCreator : public Creator<BaseType>
{
public:
	BaseType* create()
	{
		return new DerivedType;
	}
};


template <class BaseType, class Key>
class ItemFactory
{
public:
	void register_item(Key Id, Creator<BaseType>* Fn)
	{
		this->function_map_[Id] = Fn;
	}

	BaseType* create(Key Id)
	{
		return this->function_map_[Id]->create();
	}

	~ItemFactory()
	{
		std::map<Key, Creator<BaseType>*>::iterator i = this->function_map_.begin();
		while (i != this->function_map_.end())
		{
			delete (*i).second;
			++i;
		}
	}

private:
	std::map<Key, Creator<BaseType>*> function_map_;
};

using ItemFactoryType = ItemFactory<Item, evil::RareType>;
using ItemFactoryPtr = std::shared_ptr<ItemFactoryType>;