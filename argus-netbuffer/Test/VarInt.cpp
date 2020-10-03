#include "gtest/gtest.h"

#include "argus-netbuffer/netutils.h"

void TestVarInt(uint64_t num) {
	if (num < 0) {
		return;
	}
	uint8_t buf[10];
	ArgusNetUtils::writeVarInt(buf, num);

	uint64_t out = -1;
	ArgusNetUtils::readVarInt(buf, out);
	EXPECT_EQ(num, out);
}

void TestSignedVarInt(int64_t num) {
	uint8_t buf[10];
	ArgusNetUtils::writeVarIntSigned(buf, num);

	int64_t out = -1;
	ArgusNetUtils::readVarIntSigned(buf, out);
	EXPECT_EQ(num, out);
}

TEST(VarIntTest, test1) {
	for (int j = 0; j < 6; ++j) {
		uint64_t base = 1;
		for (int i = 0; i < 8; ++i) {
			TestVarInt(base - 4);
			TestVarInt(base - 3);
			TestVarInt(base - 2);
			TestVarInt(base - 1);
			TestVarInt(base - 0);
			TestVarInt(base + 1);
			base *= 128;
		}
	}

	for (int j = 0; j < 6; ++j) {
		uint64_t base = 1;
		for (int i = 0; i < 8; ++i) {
			TestSignedVarInt(base - 1);
			TestSignedVarInt(base - 0);
			TestSignedVarInt(base + 1);
			TestSignedVarInt(-base - 1);
			TestSignedVarInt(-base - 0);
			TestSignedVarInt(-base + 1);
			base *= 64;
		}
	}
}
