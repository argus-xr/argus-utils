#include "argus-transmit/QueueSingle.h"

NetMessageOut* QueueSingle::popNextMessage() {
	return currentMessage.exchange(nullptr);
}

uint64_t QueueSingle::nextMessageTimestamp() {
	return messageTimestamp.load(); // this won't be atomic together with currentMessage, but that will just cause rare instances of messages being sent out of priority order.
}

void QueueSingle::pushMessage(NetMessageOut* message, uint64_t timestamp) {
	NetMessageOut* last = currentMessage.exchange(message);
	if (last) {
		delete last;
	}
	messageTimestamp.store(timestamp);
}