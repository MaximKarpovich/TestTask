#pragma once

#include "external_api/btc_miner.h"

class BTCMiner : public IBTCMiner {
public:
	BTCMiner() { };
	~BTCMiner() {};

	float get_api_btc() { return get_btc();  };
};