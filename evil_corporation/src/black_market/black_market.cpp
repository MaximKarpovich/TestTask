#include "black_market.h"

#include "items/coin/coin.h"
#include "items/helm/helm.h"
#include "items/jug/jug.h"
#include "items/sword/sword.h"

#include "../common/thread_pool/thread_pool.h"

#include "../common/exceptions/memory_exception.h"
#include "../common/exceptions/api_access_exception.h"

#include "../common/api/api_singleton.h"

#include <iostream>
#include <functional>
#include <random>

/*Declaration of static ApiSingleton variables*/
byte *ApiSingleton::api_item;
size_t ApiSingleton::api_item_size;

BlackMarket::BlackMarket() {
	try {
		/*Initialization of singleton*/
		ApiSingleton::get_instance();
		
		this->items_factory_ = std::make_shared<ItemFactoryType>();
		if (this->items_factory_ == nullptr)
			throw MemoryException("BlackMarket could not create factory");

		std::cout << "BlackMarket started" << std::endl;
		/*Register all available item types in fabric*/
		this->items_factory_->register_item(evil::RareType::Coin, new DerivedCreator<Coin, Item>);
		this->items_factory_->register_item(evil::RareType::Helm, new DerivedCreator<Helm, Item>);
		this->items_factory_->register_item(evil::RareType::Jug, new DerivedCreator<Jug, Item>);
		this->items_factory_->register_item(evil::RareType::Sword, new DerivedCreator<Sword, Item>);

	}
	catch (MemoryException const &ex) {
		std::cerr << "MemoryException: " << ex.what() << std::endl;
		throw;
	}
}

BlackMarket::~BlackMarket() {
	/*Delete all clients and wait working threads.*/
	this->client_list_.clear();
}

ClientId BlackMarket::subscribe(ClientPtr pNewClient)
{
	boost::lock_guard<boost::mutex> lock(this->mutex_);

	size_t prevSize = this->client_list_.size();

	for (size_t i = 0; i < this->client_list_.size(); ++i)
	{
		if (this->client_list_[i]->get_subscriver_id() == pNewClient->get_subscriver_id())
		{
			return 0;
		}
	}

	/*Now client can get messages from black market*/
	this->client_list_.push_back(pNewClient);
	std::cout << "New client(id = " << pNewClient->get_subscriver_id() << ") subscribed on black market`s updates" << std::endl;

	/*When adding the first subscriber start trading*/
	if (prevSize == 0) {
		this->start_trading();
	}

	return pNewClient->get_subscriver_id();
}
bool BlackMarket::unsubscribe(ClientId id)
{
	ClientPtr toRelease;
	boost::lock_guard<boost::mutex> lock(this->mutex_);

	for (size_t i = 0; i < this->client_list_.size(); ++i) {
		if (this->client_list_[i]->get_subscriver_id() == id) {
			toRelease = this->client_list_[i];
			this->client_list_.erase(this->client_list_.begin() + i);
			std::cout << "Client (id = " << toRelease->get_subscriver_id() << ") unsubscribed from black market`s updates" << std::endl;

			/*When no clients left all threads will be stopped and trading will pause*/
			if (this->client_list_.size() == 0) {
				std::cout << "Black market lost it`s last client. It will finish operations and will be closed" << std::endl;
			}
			return true;
		}
	}

	return false;
}

void BlackMarket::start_trading() {
	std::cout << "Black market starts trading" << std::endl;
	ThreadPool::get_instance().enqueue_continuous_task(&BlackMarket::generate_item, this);
}

void BlackMarket::generate_item() {
	try {
		while (this->client_list_.size() != 0) {
			ApiSingleton::get_instance().generate_new_item();

			ItemPtr item(this->items_factory_->create((evil::RareType)(ApiSingleton::api_item[0] & 0x0F)));

			item->set_item_info(ApiSingleton::api_item, ApiSingleton::api_item_size);

			std::cout << "Black market produces new item - " << item->get_description().c_str() << std::endl;

			{
				boost::lock_guard<boost::mutex> lock(this->mutex_);

				this->notify(item);
				this->collect_responses();
				this->send_item(item);
			}
		}
	}
	catch (MemoryException const &ex) {
		std::cerr << "BlackMarket: " << ex.what() << std::endl;

	}
	catch (ApiAccessException const &ex) {
		std::cerr << "BlackMarket: " << ex.what() << std::endl;
	}
}

void BlackMarket::notify(ItemPtr item) {
	for (auto &&client : this->client_list_) {
		client->message_handler(item);
	}
}

void BlackMarket::collect_responses() {
	this->clients_ready_to_buy_.clear();

	for (auto &&client : this->client_list_) {
		if (client->get_buy_status() == true) {
			this->clients_ready_to_buy_.emplace_back(client);
		}
	}
}

void BlackMarket::send_item(ItemPtr item) {
	if (this->clients_ready_to_buy_.size() != 0) {
		std::random_device randDevice;
		std::mt19937 mt(randDevice());
		std::uniform_int_distribution<int> distribution(0, this->clients_ready_to_buy_.size() - 1);

		int winner = distribution(mt);
		this->clients_ready_to_buy_.at(winner)->receive_item(item);
	}
}