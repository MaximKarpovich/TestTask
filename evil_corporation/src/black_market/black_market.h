#pragma once

#include "items/base/base_item.h"
#include "items_factory/item_factory.h"
#include "../client/base/base_client.h"

#include <boost/thread/thread.hpp>

/*Observer pattern. This class serves as a service that supports 
  subscribing/unsubscribing on/from it`s messages. This class also is responsible for 
  generating new Item and sharing it between all subscribers. After sharing information about 
  newly generated item, it collects responses from subscribers that want to by item and with the 
  random method chooses the one person to sale. It emulates the behaviour as when someone was the first to
  click "buy"*/

class BlackMarket
{
public:
	BlackMarket();
	~BlackMarket();

	ClientId subscribe(ClientPtr pNewClient);
	bool unsubscribe(ClientId id);

private:
	/*Begin generating items*/
	void start_trading();

	/*Tell all subscribers that new item is available*/
	void notify(ItemPtr);

	/*Send a pointer to new item*/
	void send_item(ItemPtr);

	/*Collect answers from clients about readiness to buy item*/
	void collect_responses();

	/*Generate new rare item*/
	void generate_item();

private:
	/*Storage for all subscribed clients*/
	ClientsList client_list_;

	/*Storage for all subscribed clients that ready to by current item*/
	ClientsList clients_ready_to_buy_;

	/*IPC for accessing shared objects*/
	boost::mutex mutex_;

	/*Templated factory for creating needed item*/
	ItemFactoryPtr items_factory_;
};