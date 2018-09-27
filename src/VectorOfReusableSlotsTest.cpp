#include "gmock/gmock.h"
#include "VectorOfReusableSlots.h"

using namespace testing;

class VectorOfReusableSlotses:
	public testing::Test {
		public:
			VectorOfReusableSlots<int> vors; 
			int testValue = 23;
	};

TEST_F(VectorOfReusableSlotses, EmptyWhenCreated) {
	ASSERT_THAT(vors.size(), Eq(0));
}

TEST_F(VectorOfReusableSlotses, AddingItemsIncreasesSize) {
	vors.getFreeSlot(testValue);
	ASSERT_THAT(vors.size(), Eq(1));
}

TEST_F(VectorOfReusableSlotses, EmptySlotsAreFilledFirst) {
	vors.getFreeSlot(testValue);
	vors.getFreeSlot(testValue);
	vors.getFreeSlot(testValue);

	size_t freeSlotNumber = 1;
	vors.markUnused(freeSlotNumber);
	
	ASSERT_THAT(vors.getFreeSlot(testValue), Eq(freeSlotNumber));
}


