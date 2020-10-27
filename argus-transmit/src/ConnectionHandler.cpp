#include "argus-transmit/ConnectionHandler.h"
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <math.h>

ConnectionHandler::ConnectionHandler(ConnectionBase* connectionToUse, BasicMessageBuffer* bmbuf) {
	connection = connectionToUse;
	if (!connection) {
		throw std::runtime_error("ConnectionToUse is null.");
	}
	this->bmbuf = bmbuf;
	currentPacket = nullptr;
	packetBuf = new uint8_t[outBufSize];
}

void ConnectionHandler::poll() {
	while (true) {
		if (!currentPacket) {
			if (!getNewPacket()) {
				break; // nothing more to send.
			}
		}
		uint16_t bytesToAdd = std::min((int16_t) (currentPacket->getDataLength()) - currentPacketSpot, ((int16_t) outBufSize) - packetBufSpot);
		std::memcpy(packetBuf + packetBufSpot, currentPacket->getData() + currentPacketSpot, bytesToAdd);
		packetBufSpot += bytesToAdd;
		currentPacketSpot += bytesToAdd;

		if (currentPacketSpot >= currentPacket->getDataLength()) {
			delete currentPacket;
			currentPacket = nullptr;
		}
	}
	uint32_t bytesSent = connection->send(packetBuf, packetBufSpot);

	uint8_t* newBuf = new uint8_t[outBufSize];
	std::memcpy(newBuf, packetBuf + bytesSent, packetBufSpot - bytesSent);
	delete[] packetBuf;
	packetBuf = newBuf;
}

bool ConnectionHandler::getNewPacket() {
	std::vector<std::tuple<std::shared_ptr<QueueBase>, uint64_t>> sorted(queues.size());
	for (auto queue : queues) {
		sorted.push_back(std::make_tuple(queue, queue->nextMessageTimestamp()));
	}
	std::sort(sorted.begin(), sorted.end(), [](auto pairA, auto pairB) -> bool {
		return std::get<1>(pairA) > std::get<1>(pairB);
		}
	);
	for (auto [queue, time] : sorted) {
		NetMessageOut* out = queue->popNextMessage();
		if (out) {
			currentPacket = bmbuf->messageToOutPacket(out);
			if (currentPacket) {
				break;
			}
		}
	}
	return (bool)currentPacket; // do we have a new packet?
}