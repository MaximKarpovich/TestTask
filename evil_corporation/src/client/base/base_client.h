#pragma once

#include "../../black_market/items/base/base_item.h"

#include <vector>

class BaseClient;

using ItemsList = std::unique_ptr<std::vector<ItemPtr>>;
using ClientPtr = std::shared_ptr<BaseClient>;
using ClientsList = std::vector<ClientPtr>;
using ClientId = unsigned __int64;

/*Base class for all clients*/
class BaseClient
{
public:
	BaseClient() {};
	virtual ~BaseClient() {}

	/*Function that processes messages from black market*/
	virtual void message_handler(ItemPtr) = 0;

	/*Function that processes item passed from black market*/
	virtual void receive_item(ItemPtr) = 0;

	/*FUnction that returns the readiness of client to buy item*/
	virtual bool get_buy_status() = 0;

	/*Function that returns unique client id*/
	ClientId get_subscriver_id() { return (ClientId)this; }
};