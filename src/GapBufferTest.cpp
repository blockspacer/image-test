#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GapBuffer.h"

using namespace testing;

class EmptyGapBuffer:
	public testing::Test { // derived from testing::Test
		public:
			GapBuffer<int> gb;
			int testValue = 23;
	};

TEST_F(EmptyGapBuffer, IsEmptyOnCreation) {	ASSERT_THAT(gb.size(), Eq(0)); }

TEST_F(EmptyGapBuffer, RejectsNegativeIndexes) { ASSERT_FALSE(gb.validIndex(-1)); }

TEST_F(EmptyGapBuffer, RejectsTooLargeIndexes) { ASSERT_FALSE(gb.validIndex(0)); }

TEST_F(EmptyGapBuffer, AddingItemIncreasesSize) { 
	gb.insertAtGap(testValue);
	ASSERT_THAT(gb.size(), Eq(1));
}

TEST_F(EmptyGapBuffer, AcceptsValidIndexes) { 
	gb.insertAtGap(testValue);
	ASSERT_TRUE(gb.validIndex(0));
}

class ThreeItemGapBuffer:
	public testing::Test {
		public:
			GapBuffer<int> gb;
			int itemZero = 5;
			int itemOne  = 6;
			int itemTwo  = 7;
			ThreeItemGapBuffer() {
				gb.insertAtGap(itemZero);
				gb.insertAtGap(itemOne);
				gb.insertAtGap(itemTwo);
			}
			int testValue = 23;
	};

TEST_F(ThreeItemGapBuffer, RejectsTooLargeIndexes) { ASSERT_FALSE(gb.validIndex(3)); }

TEST_F(ThreeItemGapBuffer, AcceptsValidIndexes) { ASSERT_TRUE(gb.validIndex(2)); }

TEST_F(ThreeItemGapBuffer, RemovingItemReducesSize) { 
	gb.remove(1);
	ASSERT_THAT(gb.size(), Eq(2));
}

TEST_F(ThreeItemGapBuffer, InsertedItemsAreRemembered) { 
	gb.insertAt(1, testValue);
	ASSERT_THAT(gb[1], Eq(testValue));
}
