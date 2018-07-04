#pragma once

#include "../black_market/black_market.h"
#include "../black_market/items/base/base_item.h"

#include "../common/thread_pool/thread_pool.h"

#include "../common/api/miner.h"

/*Client of black market*/
class Client : public BaseClient {
public:
	Client();
	~Client();
	virtual void message_handler(ItemPtr item);
	virtual void receive_item(ItemPtr item);
	virtual bool get_buy_status();
private:
	/*Function that creates threads for mining*/
	void create_mining_threads();

	/*Function that is passed to thread to perform mining*/
	void start_mining();
private:
	/*Ready to buy flag*/
	bool ready_to_buy_;

	/*Flag that signals threads to stop*/
	bool stop_threads_;

	/*Client`s balance*/
	float money_;

	/*Mutex for shared funcs*/
	boost::mutex mutex_;

	/*Mutex for mMoney*/
	boost::mutex mining_mutex_;

	/*Object that performs mining*/
	std::unique_ptr<BTCMiner> btc_miner_;

	/*Client`s bought items*/
	ItemsList item_list_;
};