#ifndef BASICMESSAGEPROTOCOL_H
#define BASICMESSAGEPROTOCOL_H

#include "../netbuffer.h"

class BasicMessageBuffer : NetBuffer {
public:
	void checkMessages();
	NetMessage* popMessage();
protected:
	NetMessage** messageList = nullptr;
	uint8_t messageListNum = 0;
	uint8_t messageListMax = 5;
	uint8_t messageListPos = 0;
	const uint8_t delimiter[2] = { '\\', '\0'};
	void resizeMessageList(uint8_t size);
};

#endif // BASICMESSAGEPROTOCOL_H