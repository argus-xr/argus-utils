#include "BasicMessageProtocol.h"
#include <cstring>

void BasicMessageBuffer::checkMessages() {
	if (messageListNum < 250) { // don't overflow the message array
		int32_t pos = findByteSequence(startSequence, startSequenceLength, 0);
		int32_t chop = -1;
		if (pos == 0) {
			bool posFound = false;
			int32_t endpos = 1;
			while (!posFound && endpos > 0) { // endpos < 0 means there's no more matching sequences.
				endpos = findByteSequence(endSequence, endSequenceLength, endpos);
				if (endpos > 0 && getByteAt(endpos - 1) != '\\') { // check if the \ character is escaped, which would make the \0 not count.
					posFound = true;
				}
			}
			if (posFound) {
				chop = endpos + endSequenceLength;
				//TODO: read message length, which is the 2 bytes after the delimiter.
				int32_t messageLength = endpos - 4; // 2-byte start sequence and 2 bytes of length
				if (messageLength > 0) {
					uint8_t* nmbuf = new uint8_t[messageLength];
					std::memcpy(nmbuf, internalBuffer + 4, messageLength);
					NetMessageIn* newMessage = new NetMessageIn(nmbuf, messageLength);
					if (messageList == nullptr || messageListNum >= messageListMax) {
						resizeMessageList(messageListMax + 10);
					}
					messageList[messageListNum] = newMessage;
					messageListNum += 1;
				}
			}
		}
		else if (pos > 0) {
			chop = pos;
		}
		else if (internalBufferContentSize > 100) { // there's 100 bytes worth of garbage in here that won't ever match the protocol, ditch it!
			chop = internalBufferContentSize - startSequenceLength; // there could be a partial valid delimiter at the end, so don't delete everything.
		}
		if (chop > 0) {
			removeStartOfBuffer(chop);
		}
	}
}

NetMessageIn* BasicMessageBuffer::popMessage() {
	if (messageList != nullptr && messageListPos < messageListNum) {
		NetMessageIn* ret = messageList[messageListPos];
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
		NetMessageIn** tmp = messageList;
		messageList = new NetMessageIn *[size];
		if (tmp != nullptr) {
			std::memcpy(messageList, tmp + messageListPos, messageListNum - messageListPos);
			delete[] tmp;
		}
	}
}