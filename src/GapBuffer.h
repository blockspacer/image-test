#pragma once

#include <vector>

using std::vector;

#include <iostream>

using std::cout;
using std::endl;


template <typename T>
class GapBuffer {
	// note that the back part of the buffer is in reverse order, as items are most efficiently added or removed from the end of a vector
	vector<T> front, back;
	void moveGap(size_t i) {
		while (front.size() > i) {
			back.push_back(front.back());
			front.pop_back();
		}
		while (front.size() < i) {
			front.push_back(back.back());
			back.pop_back();
		}
	}
	void printVector(vector<T> const &v) const {for (size_t i=0;i<v.size();++i){cout<<v[i];}}
	void printContents() {
		cout<<"Front: ";printVector(front);
		cout<<"\nBack:  ";printVector(back);cout<<endl;
	}
public:
	size_t size() const    { return front.size() + back.size(); }
	bool validIndex(int i) { return i >= 0 && i < (int) size(); }
	void insertAtGap(T item) { front.push_back(item); }
	void insertAt(int i, T item) {
		if (! (validIndex(i) || i == (int) size())) {
			return;
		}
		moveGap(i);
		insertAtGap(item);
	}
	void remove(int i) {
		moveGap(i + 1);
		front.pop_back();
	}

	const T& operator[] (size_t i) const {
		if (i < front.size())
			return front[i];
		return back[back.size() - 1 - (i - front.size())];
	};

	T& operator[] (size_t i) {
		if (i < front.size())
			return front[i];
		return back[back.size() - 1 - (i - front.size())];
	}

	void replaceAt(int i, T item) {
		if (i < front.size())
			front[i] = item;
		else
			back[back.size() - 1 - (i - front.size())] = item;
	}
};
/*
struct Cursor {
	size_t tokenPosition, characterPosition;
	int desiredXPosition;
};

struct Excerpt {
	size_t BufferId {0};
	size_t  startPosition {0}, length {0}
		,	startLineNumber {0}, newlinesContained {0};
	vector<Cursor> cursors;
	bool containsWholeFile {false};
};

struct Token {

};

enum UndoableActionType {INSERT, REPLACE, DELETE, BEGIN_MULTISTEP_ACTION, END_MULTISTEP_ACTION};

struct UndoableAction {
	size_t position {0};
	UndoableActionType type;
	size_t data;
};

class TokenizingExcerptableBuffer {
	VectorOfReusableSlots<Excerpt> excerpts;

};
*/
