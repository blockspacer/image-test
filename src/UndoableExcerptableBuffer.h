#pragma once

#include "GapBuffer.h"
#include "VectorOfReusableSlots.h"
#include "globals.h"

#include <string>
using std::string;

using ExcerptId = size_t;
using Position  = size_t;


enum UndoableActionType {INSERT, REPLACE, REMOVE, EXTEND_EXCERPT_FRONT, RETRACT_EXCERPT_FRONT, EXTEND_EXCERPT_BACK, RETRACT_EXCERPT_BACK};

using UndoMultistepGeneration = size_t;

template <typename T>
struct UndoableAction {
	UndoableAction(UndoableActionType t, T d, UndoMultistepGeneration umg) : type(t), data(d), editGeneration{umg} {}
	UndoableAction(const UndoableAction<T> &ua) : type(ua.type), data(ua.data), editGeneration(ua.editGeneration), position(ua.position) {}
	UndoMultistepGeneration editGeneration {0};
	Position position {0};
	UndoableActionType type;
	T data;

};

template <typename T>
struct Excerpt {
	Position startPosition, length;
	vector<UndoableAction<T>> undoHistory;

	Excerpt(Position pos, Position len) : startPosition(pos), length(len) {}

	bool containsPosition(Position pos) {
		return pos >= startPosition && pos < startPosition + length;
	}
};

template <typename T>
class UndoableExcerptableBuffer {
	GapBuffer<T> myThings; 
	VectorOfReusableSlots<Excerpt<T>> myExcerpts;
	vector<UndoableAction<T>> myRedoBuffer;
	ExcerptId myLastUndoer;
	UndoMultistepGeneration myLatestEditGeneration {0};
	bool myDontGroupUndosFlag {true};

	void increaseExcerptStartPointsAtAndAfterInsertionPointCommaLengthensExcerptsStillCoveringInsertionPoint(Position firstAffectedPosition) {
		myExcerpts.forEach([&](Excerpt<T> &ex){
			if (ex.startPosition >= firstAffectedPosition)
				++ ex.startPosition;
			if (ex.containsPosition(firstAffectedPosition))
				++ ex.length;
		});
	}
	void reduceExcerptStartsPointsAndLengths(Position removalPosition) {
		myExcerpts.forEach([&](Excerpt<T> &ex){
			if (ex.containsPosition(removalPosition))
				-- ex.length;
			else if (ex.startPosition > removalPosition)
				-- ex.startPosition;
		});
	}

	void removeFromExcerptWithoutRecordingToUndoHistories(ExcerptId exId, Position excerptPosition) {
		Position bufferPosition = myExcerpts[exId].startPosition + excerptPosition;
		myThings.remove(bufferPosition);
		reduceExcerptStartsPointsAndLengths(bufferPosition);
	}

	void insertInExcerptWithoutRecordingToUndoHistories(ExcerptId exId, Position excerptPosition, T item) {
		Position bufferPosition = myExcerpts[exId].startPosition + excerptPosition;
		myThings.insertAt(bufferPosition, item);
		increaseExcerptStartPointsAtAndAfterInsertionPointCommaLengthensExcerptsStillCoveringInsertionPoint(bufferPosition);
	}	
	void replaceInExcerptWithoutRecordingToUndoHistories(ExcerptId exId, Position excerptPosition, T data) {
		myThings.replaceAt(myExcerpts[exId].startPosition + excerptPosition, data);
	}

public:
	//todo: rememberto clear redo buffer when focus changes between non-equivalent excerpts
	void clearRedoBuffer() {myRedoBuffer.clear();}
	void extendExcerptAtBack(Position formerLastBufferPosition) {
		myExcerpts.forEach([&](Excerpt<T> &ex){
			if (ex.startPosition + ex.length == formerLastBufferPosition)
				++ ex.length;
		});
	}
	void extendExcerptsForwards(Position startPosition) {
		myExcerpts.forEach([&](Excerpt<T> &ex){
			if (ex.startPosition == startPosition)
				-- ex.startPosition;
		});
	}
	void insertAtGap(T item) { myThings.insertAtGap(item); }
	void insertInExcerpt(ExcerptId exId, Position excerptPosition, T item, bool doClearRedoBuffer=true) {
		if (doClearRedoBuffer)
			clearRedoBuffer();

		insertInExcerptWithoutRecordingToUndoHistories(exId, excerptPosition, item);


		UndoableAction<T> act(REMOVE, item, myLatestEditGeneration);
		addToUndoHistories(act, myExcerpts[exId].startPosition + excerptPosition);
	}
	void removeFromExcerpt(ExcerptId exId, Position excerptPosition, bool doClearRedoBuffer=true) {
		if (doClearRedoBuffer)
			clearRedoBuffer();

		auto &ex = myExcerpts[exId];
		T data = myThings[ ex.startPosition + excerptPosition ];
		removeFromExcerptWithoutRecordingToUndoHistories(exId, excerptPosition);

		UndoableAction<T> act(INSERT, data, myLatestEditGeneration);
		addToUndoHistories(act, myExcerpts[exId].startPosition + excerptPosition);
	}
	void replaceInExcerpt(ExcerptId exId, Position excerptPosition, T item, bool doClearRedoBuffer=true) {
		if (doClearRedoBuffer)
			clearRedoBuffer();

		Position bufferPosition = myExcerpts[exId].startPosition + excerptPosition;
		T oldData = myThings[ bufferPosition ];

		replaceInExcerptWithoutRecordingToUndoHistories(exId, excerptPosition, item);

		UndoableAction<T> act(REPLACE, oldData, myLatestEditGeneration);
		addToUndoHistories(act, bufferPosition);
	}

	void addToUndoHistories(UndoableAction<T> &act, Position bufferPosition) {
		myExcerpts.forEach([&](Excerpt<T> &ex){
			if (ex.containsPosition(bufferPosition)) {
				act.position = bufferPosition - ex.startPosition;
				ex.undoHistory.push_back(act);
			}
		});
		if (myDontGroupUndosFlag)
			++ myLatestEditGeneration;
	}

	void purgeActFromUndoHistories(UndoMultistepGeneration generation) {
		myExcerpts.forEach(
			[&](Excerpt<T> &ex) {
				ex.undoHistory.erase(
					std::remove_if(
						ex.undoHistory.begin(),
						ex.undoHistory.end(), 
						[&](UndoableAction<T> ua) {
							 return ua.editGeneration == generation;
						}), 
				ex.undoHistory.end());
			});
	}


	void dontGroupUndos() {
		myDontGroupUndosFlag = true;
	}
	void doGroupUndos() {
		myDontGroupUndosFlag = false;
	}

	void undo(ExcerptId exId) {
		myLastUndoer = exId;
		Excerpt<T> &ex = myExcerpts[exId];

		if (ex.undoHistory.empty())
			return;


		UndoableAction<T> act = ex.undoHistory.back();
		Position bufferPosition = myExcerpts[exId].startPosition + act.position;
		UndoMultistepGeneration gen = act.editGeneration;


		auto actIter = ex.undoHistory.rbegin();

		UndoableAction<T> redoAct(act);
		auto iterEnd = ex.undoHistory.rend();

		while (actIter != iterEnd && actIter->editGeneration == gen) {
			redoAct.editGeneration = actIter->editGeneration;
			redoAct.position = actIter->position - ex.startPosition;
			switch (actIter->type) {
				case REMOVE:
					redoAct.type = INSERT;
					redoAct.data = lookup(exId, actIter->position);
					myRedoBuffer.push_back(redoAct);
					removeFromExcerptWithoutRecordingToUndoHistories(exId, actIter->position);
					break;
				case INSERT:
					redoAct.type = REMOVE;
					myRedoBuffer.push_back(redoAct);
					redoAct.editGeneration = actIter->editGeneration;
					insertInExcerptWithoutRecordingToUndoHistories(exId, actIter->position, actIter->data);
					break;
				case REPLACE:
					redoAct.type = REPLACE;
					redoAct.data = lookup(exId, actIter->position);
					myRedoBuffer.push_back(redoAct);
					replaceInExcerptWithoutRecordingToUndoHistories(exId, actIter->position, actIter->data);
					break;
			}
			++ actIter;
		}
		purgeActFromUndoHistories(gen);
	}

	void redo() {
		if (myRedoBuffer.empty())
			return;

		UndoableAction<T> act = myRedoBuffer.back();
		myRedoBuffer.pop_back();

		switch (act.type) {
			case REMOVE:
				removeFromExcerpt(myLastUndoer, act.position, false);
				break;
			case INSERT:
				insertInExcerpt(myLastUndoer, act.position, act.data, false);
				break;
			case REPLACE:
				replaceInExcerpt(myLastUndoer, act.position, act.data, false);
				break;
		}
	}



	Position excerptLength(ExcerptId exId) {return myExcerpts[exId].length;}
	Position excerptStart (ExcerptId exId) {return myExcerpts[exId].startPosition;}
	size_t size() const {return myThings.size();}
	T& operator[] (size_t i) { return myThings[i]; }
	const T& operator[] (size_t i) const {return myThings[i];};
	ExcerptId makeExcerpt(size_t start, size_t len) {
		Excerpt<T> newExcerpt{start, len};
		return myExcerpts.getFreeSlot(newExcerpt);
	}
	ExcerptId makeExcerptContainingEntireBuffer() {
		Excerpt<T> newExcerpt{0, size()};
		return myExcerpts.getFreeSlot(newExcerpt);		
	}
	const T& lookup(const ExcerptId id, const size_t position) const {
		return myThings[ myExcerpts[id].startPosition + position ];
	}
	const T& lookup(const size_t position) const {
		return myThings[ position ];
	}
};


