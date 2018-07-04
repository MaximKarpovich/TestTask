#pragma once

#include "external_api/api.h"
#include "../exceptions/api_access_exception.h"
#include "../exceptions/memory_exception.h"

#include <Windows.h>
#include <iostream>
#include <mutex>

using apiPtr = evil::api(*);

/*Api singleton pattern. Gets an instance of dll library and uses it`s exported function*/
class ApiSingleton {
private:
	ApiSingleton() {
		try {
			auto hd1 = LoadLibraryA("../../external_api/lib/api.dll");
			if (hd1) {
				api_interface_ = reinterpret_cast<apiPtr>(GetProcAddress(hd1, "api"));

				if (!api_interface_) {
					throw ApiAccessException("Could not get proc address");
				}
			}
			else
				throw MemoryException("Could not load api.dll");
		}
		catch (MemoryException const &ex) {
			std::cerr << "ApiSingleton: " << ex.what() << std::endl;
			throw;
		}
		catch (ApiAccessException const &ex) {
			std::cerr << "ApiSingleton: " << ex.what() << std::endl;
			throw;
		}
	}
	~ApiSingleton() {};

private:
	apiPtr api_interface_;

public:
	ApiSingleton(ApiSingleton const&) = delete;
	ApiSingleton& operator=(ApiSingleton const&) = delete;

	/*Create new rare item. Means invoke api generator function with custom callback*/
	void generate_new_item() {
		api_interface_->get_raw_rare(&ApiSingleton::process_item);
	}

	/*Save currently generated rare thing*/
	static void process_item(const byte *item, const size_t size) {
		ApiSingleton::api_item = (byte*)malloc(size);
		strcpy_s((char *)ApiSingleton::api_item, size, (char *)item);
		api_item_size = size;
	};

	static ApiSingleton &get_instance() {
		static ApiSingleton instance_;
		return instance_;
	}

public:
	static byte *api_item;
	static size_t api_item_size;
};