#ifndef NETUTILS_H
#define NETUTILS_H

// htons/htonl
#if defined(_WIN32)
#include <winsock2.h>
#elif defined(arduino_h)
#include <WiFi.h>
#else
#include <arpa/inet.h>
#endif

namespace ArgusNetUtils {
	uint16_t toNetworkOrder(uint16_t val) {
		return htons(val);
	}
	uint16_t toHostOrder(uint16_t val) {
		return ntohs(val);
	}
	uint32_t toNetworkOrder(uint32_t val) {
		return htonl(val);
	}
	uint32_t toHostOrder(uint32_t val) {
		return ntohl(val);
	}
}

#endif // NETUTILS_H