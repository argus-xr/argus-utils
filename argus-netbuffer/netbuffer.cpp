#include "netbuffer.h"
#include <cstring>


// htons/htonl
#if defined(_WIN32)
#include <winsock2.h>
#elif defined(arduino_h)
#include <WiFi.h>
#else
#include <arpa/inet.h>
#endif

void NetBuffer::insertBuffer(uint8_t* buffer, uint32_t length, bool copyBuffer) {
	uint32_t neededSize = length + internalBufferContentSize;
	if (internalBuffer == nullptr || neededSize > internalBufferMemorySize) {
		uint8_t* oldBuffer = internalBuffer;
		internalBuffer = new uint8_t[neededSize + bufferResizeStep];
		if (oldBuffer != nullptr) {
			std::memcpy(internalBuffer, oldBuffer, internalBufferContentSize);
		}
		internalBufferMemorySize = neededSize + bufferResizeStep;
	}
	std::memcpy(internalBuffer + internalBufferContentSize, buffer, length);
	internalBufferContentSize += length;

	if (!copyBuffer) {
		// This is not actually supported yet, but if false, the inserted buffer is handed over.
		// That means the caller is no longer responsible for it, so we're deleting it here.
		// This will probably be supported in later, more efficient implementations.
		delete[] buffer;
	}
}

void NetBuffer::removeStartOfBuffer(uint32_t length) {
	if (internalBuffer == nullptr) {
		return;
	}
	else if (length >= internalBufferContentSize) {
		delete[] internalBuffer;
		internalBuffer = nullptr;
		internalBufferContentSize = 0;
		internalBufferMemorySize = 0;
	}
	else {
		uint8_t* oldBuffer = internalBuffer;

		internalBufferContentSize = internalBufferContentSize - length; // don't need the old values anymore.
		internalBufferMemorySize = internalBufferContentSize + bufferResizeStep;

		internalBuffer = new uint8_t[internalBufferMemorySize];
		std::memcpy(internalBuffer, oldBuffer + length, internalBufferContentSize);
	}
}

uint32_t NetBuffer::getLength() {
	return internalBufferContentSize;
}

uint8_t NetBuffer::getByteAt(uint32_t pos) {
	if (internalBuffer != nullptr && internalBufferContentSize > pos) {
		return internalBuffer[pos];
	}
	return 0; // Internal exception handling code is not enabled by default on Arduino platform, and uses a lot of memory. Need alternative?
}

uint8_t* NetBuffer::extractBufferAt(uint32_t pos, uint32_t length) {
	if (internalBuffer != nullptr && internalBufferContentSize > pos) {
		uint8_t* result = new uint8_t[length];
		uint32_t copiedBytes = length < internalBufferContentSize ? length : internalBufferContentSize;
		memcpy(result, internalBuffer + pos, copiedBytes);
		if (copiedBytes < length) {
			memset(result, 0, length - copiedBytes);
		}
	}
	return nullptr;
}

int32_t NetBuffer::findByteSequence(const uint8_t* sequence, uint32_t sequenceLength, uint32_t startPos) {
	if (internalBuffer != nullptr) {
		for (uint32_t i = startPos; i <= internalBufferContentSize - sequenceLength; ++i) {
			for (uint32_t j = 0; j < sequenceLength; ++j) {
				if (internalBuffer[i + j] != sequence[j]) {
					break; // break J loop, the sequence isn't here.
				}
				else if (j == sequenceLength - 1) { // checked the last byte of the sequence, still matches.
					return i;
				}
			}
		}
	}
	return -1;
}

void NetBuffer::setResizeStep(uint32_t step) {
	bufferResizeStep = step;
}



// NETMESSAGE

NetMessage::NetMessage(uint8_t* buffer, uint32_t length) {
	internalBuffer = buffer;
	bufferLength = length;
}

bool NetMessage::isValid() {
	return (internalBuffer != nullptr);
}

void NetMessage::setReadPos(uint32_t pos) {
	bufferPos = pos;
}

uint8_t NetMessage::readuint8() {
	if (bufferLength < bufferPos + 1) {
		// fail
		return 0;
	}
	return internalBuffer[bufferPos];
}

uint16_t NetMessage::readuint16() {
	if (bufferLength < bufferPos + 2) {
		// fail
		return 0;
	}
	uint16_t tmp = 0;
	tmp |= internalBuffer[bufferPos] << 8;
	tmp |= internalBuffer[bufferPos + 1];
	return ntohs(tmp);
}

uint32_t NetMessage::readuint32() {
	if (bufferLength < bufferPos + 4) {
		// fail
		return 0;
	}
	uint32_t tmp = 0;
	tmp |= internalBuffer[bufferPos] << 24;
	tmp |= internalBuffer[bufferPos + 1] << 16;
	tmp |= internalBuffer[bufferPos + 2] << 8;
	tmp |= internalBuffer[bufferPos + 3];
	return ntohl(tmp);
}

uint8_t* NetMessage::getInternalBuffer() {
	return internalBuffer;
}

uint32_t NetMessage::getInternalBufferLength() {
	return bufferLength;
}