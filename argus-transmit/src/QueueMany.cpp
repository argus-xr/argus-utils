#include "argus-transmit/QueueMany.h"

NetMessageOut* QueueMany::popNextMessage() {
	mut.lock();
	if (queue.empty()) {
		mut.unlock();
		return nullptr;
	}
	auto [out, time] = queue.front();
	queue.pop();
	mut.unlock();
	return out;
}

uint64_t QueueMany::nextMessageTimestamp() {
	mut.lock();
	if (queue.empty()) {
		mut.unlock();
		return UINT64_MAX;
	}
	auto [out, time] = queue.front();
	mut.unlock();
	return time;
}

void QueueMany::pushMessage(NetMessageOut* message, uint64_t timestamp) {
	mut.lock();
	queue.push(std::make_tuple(message, timestamp));
	mut.unlock();
}

QueueMany::~QueueMany() {
	// don't bother locking mutex. If anything accesses this during the destructor, we've got other problems anyway.
	while(!queue.empty()) {
		auto [out, time] = queue.front();
		queue.pop();
		delete out;
	}
}