#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <stdint.h>
#include <vector>
#include <memory>

#include "argus-netbuffer/netbuffer.h"
#include "argus-netbuffer/BasicMessageProtocol/BasicMessageProtocol.h"
#include "QueueBase.h"

class ConnectionHandler {
protected:
	std::vector<std::shared_ptr<QueueBase>> queues;
	BasicMessageBuffer* bmbuf;
	ConnectionBase* connection;
	OutPacket* currentPacket;
	uint16_t currentPacketSpot = 0;
	const uint16_t outBufSize = 1500;
	uint8_t* packetBuf;
	uint16_t packetBufSpot = 0;

	bool getNewPacket();

public:
	ConnectionHandler(ConnectionBase* connectionToUse, BasicMessageBuffer* bmbuf);
	void poll();
};

#endif // CONNECTIONHANDLER_H