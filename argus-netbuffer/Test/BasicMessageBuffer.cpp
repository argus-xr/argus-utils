#include "gtest/gtest.h"

#include "argus-netbuffer/BasicMessageProtocol/BasicMessageProtocol.h"
#include "argus-netbuffer/netutils.h"
#include <math.h>
#include <vector>

TEST(BasicMessageProtocol, SplitMultiMessageTest) {
	BasicMessageBuffer bmbuf;

	const int numMessages = 5;
	int bytesPerMessage = 3 // 2 VarInts below 128, plus another for message length
		+ 4 // 4-character text, plus VarInt listed above
		+ bmbuf.endSequenceLength
		+ bmbuf.startSequenceLength;

	uint8_t* testBuf = new uint8_t[numMessages * bytesPerMessage * 2]; // allocate too much because this is not the place to run out of space.
	uint32_t testBufSpot = 0;

	for (int i = 0; i < numMessages; ++i) {
		NetMessageOut* out = new NetMessageOut(20);
		out->writeVarInt(5);
		out->writeVarString("Test");
		OutPacket* op = bmbuf.messageToOutPacket(out);
		delete out;

		EXPECT_EQ(op->getDataLength(), bytesPerMessage);
		memcpy(testBuf + testBufSpot, op->getData(), op->getDataLength());
		testBufSpot += op->getDataLength();
		delete op;
	}

	for (int n = 0; n < 3; ++n) {
		uint32_t skip;

		switch (n) {
		case 1: skip = bytesPerMessage; break;
		case 2: skip = bytesPerMessage * 3; break;
		default: skip = 2; break;
		}

		for (int i = 0; i < testBufSpot; i += skip) { // testBufSpot is currently the size of the buffer's contents.
			bmbuf.insertBuffer(testBuf + i, std::min(skip, testBufSpot - i), true);
			bmbuf.checkMessages();
		}

		std::vector<NetMessageIn*> messageList;
		NetMessageIn* in = nullptr;
		do {
			in = bmbuf.popMessage();
			if (in) {
				messageList.push_back(in);
			}
		} while (in);

		EXPECT_EQ(messageList.size(), numMessages);

		for (NetMessageIn* in : messageList) {
			EXPECT_EQ(in->readVarInt(), 5);
			std::string text = in->readVarString();
			EXPECT_STREQ(text.c_str(), "Test");
			delete in;
		}
	}

	delete[] testBuf;

}