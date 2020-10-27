#ifndef QUEUEBASE_H
#define QUEUEBASE_H

#include "argus-transmit/ConnectionBase.h"
#include "argus-netbuffer/netbuffer.h"

class QueueBase {
public:
	virtual NetMessageOut* popNextMessage() = 0;
	virtual uint64_t nextMessageTimestamp() = 0;
	virtual void pushMessage(NetMessageOut* message, uint64_t timestamp) = 0;
	virtual ~QueueBase() = 0;
};

#endif // QUEUEBASE_H