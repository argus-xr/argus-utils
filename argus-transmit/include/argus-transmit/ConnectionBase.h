#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

#include <tuple>
#include <stdint.h>

class ConnectionBase {
public:
	virtual ~ConnectionBase() = 0;

	virtual uint32_t send(uint8_t* buf, uint32_t buf_len) = 0;
	virtual std::tuple<uint8_t*, uint32_t> recv() = 0;
	virtual bool isConnected() = 0;
	virtual void setEnabled(bool enable) = 0;
};

#endif // CONNECTIONBASE_H