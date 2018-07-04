// EvilCorporation.cpp : Defines the entry point for the console application.
//
#include "black_market/black_market.h"
#include "client/client.h"
#include "common/exceptions/memory_exception.h"

#include <boost/thread/thread.hpp>

#include <iostream>
#include <thread>

int main()
{	
	try {
		BlackMarket blackMarket;

		ClientPtr client1(new Client);
		ClientPtr client2(new Client);

		blackMarket.subscribe(client1);
		blackMarket.subscribe(client2);

		boost::this_thread::sleep_for(boost::chrono::milliseconds(70000));
		blackMarket.unsubscribe(client1->get_subscriver_id());
		blackMarket.unsubscribe(client2->get_subscriver_id());
	}
	catch (MemoryException const &ex) {
		std::cerr << ex.what() << std::endl;
	}

	return 0;
}


