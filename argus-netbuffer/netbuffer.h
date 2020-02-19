#ifndef NETBUFFER_H
#define NETBUFFER_H

#include <cstdint>

class NetBuffer {
public:
	void insertBuffer(uint8_t* buffer, uint32_t length, bool copyBuffer);
	void removeStartOfBuffer(uint32_t length);
	void setResizeStep(uint32_t step);
	uint32_t getLength();
	uint8_t getByteAt(uint32_t pos);
	uint8_t* extractBufferAt(uint32_t pos, uint32_t length);
	int32_t findByteSequence(uint8_t* sequence, uint32_t sequenceLength, uint32_t startPos);

protected:
	uint8_t* internalBuffer = nullptr;
	uint32_t internalBufferContentSize = 0;
	uint32_t internalBufferMemorySize = 0;
	uint32_t bufferResizeStep = 128;
};

#endif