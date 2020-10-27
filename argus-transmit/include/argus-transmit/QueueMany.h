#ifndef QUEUEMANY_H
#define QUEUEMANY_H

#include "argus-transmit/QueueBase.h"
#include <atomic>
#include <queue>
#include <mutex>
#include <tuple>

class QueueMany : public QueueBase {
public:
	NetMessageOut* popNextMessage();
	uint64_t nextMessageTimestamp();
	void pushMessage(NetMessageOut* message, uint64_t timestamp);
	~QueueMany();
protected:
	std::queue<std::tuple<NetMessageOut*, uint64_t>> queue;
	std::mutex mut;
};

#endif // QUEUEMANY_H