#include "gmock/gmock.h"
#include "UndoableExcerptableBuffer.h"

using namespace testing;

#include <iostream>
using std::cout;
using std::endl;

class ExcerptableCharacterBuffer:
	public testing::Test {
		
		public:
			UndoableExcerptableBuffer<char> abcdef;

			UndoableExcerptableBuffer<char> fromString(const string &s) {
				UndoableExcerptableBuffer<char> acc;
				for (size_t i=0; i<s.size(); ++i) {
					acc.insertAtGap(s[i]);
				}
				return acc;
			}
			string toString(const UndoableExcerptableBuffer<char> &buff) {
				string acc {""};
				for (size_t i=0; i<buff.size(); ++i) {
					char c = buff[i];
					acc.append(1, c);
				}
				return acc;
			}
			void SetUp() {
				abcdef = fromString("abcdef");
			}
	};

TEST_F(ExcerptableCharacterBuffer, CanPresentItselfAsAStringForTesting) {
	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}

TEST_F(ExcerptableCharacterBuffer, CanMakeAnExcerpt) {
	ExcerptId id = abcdef.makeExcerpt(3, 3);
	ASSERT_THAT(abcdef.lookup(id, 1), Eq('e'));
}

TEST_F(ExcerptableCharacterBuffer, CanInsertThings) {
	ExcerptId id = abcdef.makeExcerptContainingEntireBuffer();
	abcdef.insertInExcerpt(id, 1, '0');
	ASSERT_THAT(toString(abcdef), Eq("a0bcdef"));
}


TEST_F(ExcerptableCharacterBuffer, InsertingThingsUpdatesSubsequentExcerptStartPositions) {
	ExcerptId id = abcdef.makeExcerpt(3, 3);
	ExcerptId all = abcdef.makeExcerptContainingEntireBuffer();
	abcdef.insertInExcerpt(all, 1, '0');
	ASSERT_THAT(abcdef.lookup(id, 1), Eq('e'));
}

TEST_F(ExcerptableCharacterBuffer, InsertingThingsUpdatesExcerptLengths) {
	Position length = 3;
	ExcerptId id  = abcdef.makeExcerpt(3, length); // abc(def)  
	ExcerptId id2 = abcdef.makeExcerpt(2, length); // ab(cde)f

	Position insertPosition = 1;
	abcdef.insertInExcerpt(id, insertPosition, '0');

	ASSERT_THAT(abcdef.excerptLength(id), Eq(4));
	ASSERT_THAT(abcdef.excerptLength(id2), Eq(4));
}

TEST_F(ExcerptableCharacterBuffer, InsertingThingsUpdatesExcerptLengths2) {
	Position length = 3;
	ExcerptId id  = abcdef.makeExcerpt(3, length); // a b c(d e f)
	ExcerptId id2 = abcdef.makeExcerpt(2, length); // a b(c d e)f
	ExcerptId id3 = abcdef.makeExcerpt(2, length); // a b(c d e)f

	// 0 1 2 3 4 5 
	// . . . . . . 
	// a b c(d e f)
	// a b(c d e)f
	// a b(c d e)f

	Position insertPosition = 0;
cout<<"a"<<endl;
	abcdef.insertInExcerpt(id2, insertPosition, '0');
cout<<"b"<<endl;
	abcdef.extendExcerptsForwards(abcdef.excerptStart(id2));

	// 0 1 2 3 4 5 6
	// . . . . . . .
	// a b 0 c(d e f)
	// a b(0 c d e)f
	// a b(0 c d e)f

	ASSERT_THAT(abcdef.excerptStart(id), Eq(4));
	ASSERT_THAT(abcdef.excerptStart(id2), Eq(2));
	ASSERT_THAT(abcdef.excerptStart(id3), Eq(2));
}

TEST_F(ExcerptableCharacterBuffer, RemovingThingsReducesExcerptLengths) {
	Position startPosition = 3, length = 3;
	ExcerptId id = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerpt(startPosition, length);
	abcdef.removeFromExcerpt(id, 1);
	ASSERT_THAT(abcdef.excerptLength(id2), Eq(2));
}

TEST_F(ExcerptableCharacterBuffer, RemovingThingsChangesLaterExcerptsStartPositions) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(0, length);
	ExcerptId id2 = abcdef.makeExcerpt(startPosition, length);
	abcdef.removeFromExcerpt(id, 1);
	ASSERT_THAT(abcdef.excerptStart(id2), Eq(2));
}

TEST_F(ExcerptableCharacterBuffer, InsertionsCanBeUndone) {
	Position startPosition = 3, length = 3;
	ExcerptId id = abcdef.makeExcerpt(startPosition, length);

	abcdef.insertInExcerpt(id, 1, '.');

	abcdef.undo(id);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}

TEST_F(ExcerptableCharacterBuffer, InsertionsCanBeUndoneFromAnyExcerptInWhichTheyreVisible) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerpt(startPosition-1, length+1);

	abcdef.insertInExcerpt(id, 1, '.');

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}

TEST_F(ExcerptableCharacterBuffer, InsertionsCanNotBeUndoneMultipleTimes) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerpt(startPosition-1, length+1);

	abcdef.insertInExcerpt(id, 1, '.');

	abcdef.undo(id);
	abcdef.undo(id2);

	vector<int> iii{1,2,3,4};
	cout<<iii.size()<<endl;

//	remove_if(iii.begin(), iii.end(), [](int i){return i<2;});
	iii.erase(std::remove_if(iii.begin(), iii.end(), [](int i){return i<2;}), iii.end());
	cout<<iii.size()<<endl;

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}


TEST_F(ExcerptableCharacterBuffer, RemovalsCanBeUndoneFromAnyExcerptInWhichTheyreVisible) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerpt(startPosition-1, length+1);

	abcdef.removeFromExcerpt(id, 1);

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}

TEST_F(ExcerptableCharacterBuffer, ReplacementsCanBeUndoneFromAnyExcerptInWhichTheyreVisible) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerpt(startPosition-1, length+1);

	abcdef.replaceInExcerpt(id, 1, '.');

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}

TEST_F(ExcerptableCharacterBuffer, UndoHistoriesAreLocalToExcerpts) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerptContainingEntireBuffer();

	abcdef.insertInExcerpt(id, 2, '.');

	ASSERT_THAT(toString(abcdef), Eq("abcde.f"));

	abcdef.insertInExcerpt(id2, 1, '_');

	ASSERT_THAT(toString(abcdef), Eq("a_bcde.f"));

	abcdef.undo(id);

	ASSERT_THAT(toString(abcdef), Eq("a_bcdef"));
}

TEST_F(ExcerptableCharacterBuffer, OverlappingExcerptsDontInterfearWithEachOthersUndoHistories) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerptContainingEntireBuffer();

	abcdef.insertInExcerpt(id, 2, '.');

	ASSERT_THAT(toString(abcdef), Eq("abcde.f"));

	abcdef.insertInExcerpt(id2, 1, '_');

	ASSERT_THAT(toString(abcdef), Eq("a_bcde.f"));

	abcdef.undo(id);

	ASSERT_THAT(toString(abcdef), Eq("a_bcdef"));

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));

}

TEST_F(ExcerptableCharacterBuffer, OverlappingExcerptsDontInterfearWithEachOthersUndoHistories2) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerptContainingEntireBuffer();
	ExcerptId id2 = abcdef.makeExcerptContainingEntireBuffer();

	abcdef.insertInExcerpt(id, 5, '.');

	ASSERT_THAT(toString(abcdef), Eq("abcde.f"));

	abcdef.insertInExcerpt(id2, 1, '_');

	ASSERT_THAT(toString(abcdef), Eq("a_bcde.f"));

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("abcde.f"));

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));

}

TEST_F(ExcerptableCharacterBuffer, OverlappingExcerptsDontInterfearWithEachOthersUndoHistories3) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);
	ExcerptId id2 = abcdef.makeExcerptContainingEntireBuffer();

	abcdef.insertInExcerpt(id2, 2, '_');

	ASSERT_THAT(toString(abcdef), Eq("ab_cdef"));

	ASSERT_THAT(abcdef.excerptStart(id), Eq(4));

	abcdef.insertInExcerpt(id, 1, '.');
	ASSERT_THAT(toString(abcdef), Eq("ab_cd.ef"));

	abcdef.undo(id2);

	ASSERT_THAT(toString(abcdef), Eq("ab_cdef"));


}

TEST_F(ExcerptableCharacterBuffer, OverlappingExcerptsDontInterfearWithEachOthersUndoHistories4) {
	Position startPosition = 3, length = 3;
	ExcerptId right = abcdef.makeExcerpt(startPosition, length);
	ExcerptId whole = abcdef.makeExcerptContainingEntireBuffer();

	abcdef.insertInExcerpt(whole, 3, '_');

	ASSERT_THAT(abcdef.excerptStart(right), Eq(4));



}


TEST_F(ExcerptableCharacterBuffer, CanRedoThings) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);

	abcdef.insertInExcerpt(id, 1, '.');

	abcdef.undo(id);
	abcdef.redo();

	ASSERT_THAT(toString(abcdef), Eq("a.bcdef"));
}

TEST_F(ExcerptableCharacterBuffer, CanRedoMultipleThings) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);

	abcdef.insertInExcerpt(id, 1, '.');
	abcdef.insertInExcerpt(id, 1, '.');

	abcdef.undo(id);
	abcdef.undo(id);
	abcdef.redo();
	abcdef.redo();

	ASSERT_THAT(toString(abcdef), Eq("a..bcdef"));
}

TEST_F(ExcerptableCharacterBuffer, CanUndoMultipleEditsInSingleStep) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);

	abcdef.doGroupUndos();
	abcdef.insertInExcerpt(id, 1, '.');
	abcdef.insertInExcerpt(id, 1, '.');
	abcdef.dontGroupUndos();
	abcdef.undo(id);

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}

TEST_F(ExcerptableCharacterBuffer, CanRedoMultipleEditsInSingleStep) {
	Position startPosition = 3, length = 3;
	ExcerptId id  = abcdef.makeExcerpt(startPosition, length);

	abcdef.doGroupUndos();
	abcdef.insertInExcerpt(id, 1, '.');
	abcdef.insertInExcerpt(id, 1, '.');
	abcdef.dontGroupUndos();
	
	abcdef.undo(id);
	abcdef.redo();

	ASSERT_THAT(toString(abcdef), Eq("abcdef"));
}
// can extend excerpts forwards
// can undo ^
// can redo ^
// can contract excerpts forwards
// can undo ^
// can redo ^
// can extend excerpts at back
// can undo ^
// can redo ^
// can contract excerpts at back
// can undo ^
// can redo ^




/**/
