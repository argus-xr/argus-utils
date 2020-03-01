#include "netutils.h"

// htons/htonl
#if defined(_WIN32)
#include <winsock2.h>
#elif defined(ESP_PLATFORM)
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
    uint8_t writeVarInt(uint8_t* buf, uint64_t val) {
        uint64_t div = 1;
        for(int i = 0; i < 10; ++i) { // 70 bits max, enough to store uint64_t
            buf[i] = (uint8_t) (val / div);
            div *= 128;
            if(val < div) {
                return i + 1; // number of bytes used
            }
        }
        return 0;
    }
    uint8_t readVarInt(uint8_t* buf, uint64_t &val) {
        // Variable-length integer; starts with least significant bytes.
        val = 0;
        uint64_t base = 1;
        for (int i = 0; i < 10; ++i) { // 70 bits max, enough to store uint64_t
            uint8_t step = buf[i];
            if (step > 127) {
                val += base * (step - 128); // Got to read another byte.
                base *= 128;
            }
            else {
                val += base * step;
                return i + 1;
            }
        }
        return 0;
    }
    uint8_t bytesToFitVarInt(uint64_t val) {
        uint64_t mult = 128;
        for(int i = 1; i <= 10; ++i) {
            if(val < mult) {
                return i;
            }
            mult *= 128;
        }
        return 10;
    }
}