#include "client.h"

#include "../common/exceptions/memory_exception.h"

#include <iostream>
#include <random>

Client::Client() {
	try {
		this->btc_miner_ = std::make_unique<BTCMiner>();

		if (!this->btc_miner_)
			throw MemoryException("Could not create BTCMiner");

		this->item_list_ = std::make_unique<std::vector<ItemPtr>>();

		std::cout << "Client (id = " << this->get_subscriver_id() << ") starts mining." << std::endl;

		this->create_mining_threads();
	}
	catch (MemoryException const &ex) {
		std::cerr << "Client: " << ex.what() << std::endl;
		throw;
	}
}

Client::~Client() {
	this->stop_threads_ = true;
}

void Client::create_mining_threads() {
	this->stop_threads_ = false;

	ThreadPool::get_instance().enqueue_continuous_task(&Client::start_mining, this);
}

void Client::start_mining() {
	float generatedBRC;
	while (this->stop_threads_ == false) {
		if (this->btc_miner_ != nullptr) {
			generatedBRC = this->btc_miner_->get_api_btc();
			{
				boost::lock_guard<boost::mutex> lock(this->mining_mutex_);

				this->money_ += generatedBRC;
			}
		}
	}
}

void Client::message_handler(ItemPtr item) {
	boost::lock_guard<boost::mutex> lock(this->mutex_);
	
	std::random_device randDevice;
	std::mt19937 mt(randDevice());
	std::uniform_int_distribution<int> distribution(0, 2);

	value cost = item->get_cost();
	/*Here can be written any rules for specific client. Based on that rules decision about buying item is made*/
	{
		boost::lock_guard<boost::mutex> lock(this->mining_mutex_);
		//if (mMoney > cost) {
			this->ready_to_buy_ = (distribution(mt) == 1) ? true : false;
		//}
	}
	
	std::cout << "Client (id = " << this->get_subscriver_id() << ") decided " << ((this->ready_to_buy_ == true) ? "to buy" : "not to buy.") << std::endl;
}

void Client::receive_item(ItemPtr item) {
	boost::lock_guard<boost::mutex> lock(this->mutex_);

	this->item_list_->emplace_back(item);

	std::cout << "Client (id = " << this->get_subscriver_id() << ") recieved item - " << item->get_description().c_str() << ". Currently he has " << this->item_list_->size() << " items in store" << std::endl;
	
	{
		boost::lock_guard<boost::mutex> lock(this->mining_mutex_);
		this->money_ -= item->get_cost();
	}
}

bool Client::get_buy_status() {
	boost::lock_guard<boost::mutex> lock(this->mutex_);

	return this->ready_to_buy_;
}

