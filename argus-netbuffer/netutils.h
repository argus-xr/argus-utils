#ifndef NETUTILS_H
#define NETUTILS_H

#include "stdint.h"

namespace ArgusNetUtils {
	uint16_t toNetworkOrder(uint16_t val);
	uint16_t toHostOrder(uint16_t val);
	uint32_t toNetworkOrder(uint32_t val);
	uint32_t toHostOrder(uint32_t val);
	uint8_t writeVarInt(uint8_t* buf, uint64_t val);
	uint8_t readVarInt(uint8_t* buf, uint64_t &val);
	uint8_t bytesToFitVarInt(uint64_t val);
}

#endif // NETUTILS_H