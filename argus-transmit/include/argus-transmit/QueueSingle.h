#ifndef QUEUESINGLE_H
#define QUEUESINGLE_H

#include "argus-transmit/QueueBase.h"
#include <atomic>

class QueueSingle : public QueueBase {
public:
	NetMessageOut* popNextMessage();
	uint64_t nextMessageTimestamp();
	void pushMessage(NetMessageOut* message, uint64_t timestamp);
	~QueueSingle();
protected:
	std::atomic<NetMessageOut*> currentMessage;
	std::atomic<uint64_t> messageTimestamp = UINT64_MAX;
};

#endif // QUEUESINGLE_H