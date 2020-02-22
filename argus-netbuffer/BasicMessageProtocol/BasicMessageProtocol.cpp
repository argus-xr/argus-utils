#include "BasicMessageProtocol.h"
#include <cstring>

void BasicMessageBuffer::checkMessages() {
	while (messageListNum < 250) { // don't overflow the message array, keep looping until break;
		int32_t pos = findByteSequence(delimiter, 2, 0);
		if (pos == 0) {
			bool posFound = false;
			int32_t endpos = 1;
			while (!posFound && endpos > 0) { // endpos < 0 means there's no more matching sequences.
				endpos = findByteSequence(delimiter, 2, endpos);
				if (getByteAt(endpos - 1) != '\\') { // check if the \ character is escaped, which would make the \0 not count.
					posFound = true;
				}
			}
			if (posFound) {
				//TODO: read message length, which is the 2 bytes after the delimiter.
				uint32_t messageLength = endpos - 6; // 2 delimiters and 2 bytes of length
				uint8_t* nmbuf = new uint8_t[messageLength - 6];
				std::memcpy(nmbuf, internalBuffer + 2, messageLength);
				NetMessage* newMessage = new NetMessage(nmbuf, messageLength);
				if (messageList == nullptr || messageListNum >= messageListMax) {
					resizeMessageList(messageListMax + 10);
				}
				messageList[messageListNum] = newMessage;
				messageListNum += 1;
			}
		}
	}
}

NetMessage* BasicMessageBuffer::popMessage() {
	if (messageList != nullptr && messageListPos < messageListNum) {
		NetMessage* ret = messageList[messageListPos];
		messageList[messageListPos] = nullptr;
		messageListPos += 1;
		if (messageListPos >= messageListMax) { // reached the end of the list
			resizeMessageList(0);
		}
		return ret;
	}
	return nullptr;
}

void BasicMessageBuffer::resizeMessageList(uint8_t size) {
	if (size == 0) {
		delete[] messageList;
		messageList = nullptr;
	}
	else {
		if (size < messageListNum) {
			size = messageListNum; // don't drop messages
		}
		NetMessage** tmp = messageList;
		messageList = new NetMessage*[size];
		if (tmp != nullptr) {
			std::memcpy(messageList, tmp + messageListPos, messageListNum - messageListPos);
			delete[] tmp;
		}
	}
}