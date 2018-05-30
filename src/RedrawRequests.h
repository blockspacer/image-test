#pragma once

#include <unordered_set>
using std::unordered_set;

#include "globals.h"

class RedrawRequests {
#define BUBBLES_MOVED (1)
#define BUBBLES_HALOS_CHANGED (1<<1)
#define WORKSPACE_VIEW_CHANGED (1<<2)
#define BUBBLE_CONTENTS_CHANGED (1<<3)

	WindowId myNewWindowParentId;
	bool     myNewWindowFlag {false};
	bool     myRedrawEverythingFlag {true};

	unordered_set<BubbleId> myChangedBubbleHalos;
	unordered_set<BubbleId> myChangedBubbleContents;
	unordered_set<WindowId> myWindowViewportChanged;
public:
	void newWindow(WindowId parent);
	void newWindowCreated();
	bool newWindowRequested() {return myNewWindowFlag;};
	WindowId newWindowParentId() {return myNewWindowParentId;};

	void redrawAllWindows() {myRedrawEverythingFlag = true;};
	bool doRedrawEverything() {return myRedrawEverythingFlag;}
	void haveRedrawnEverything() {myRedrawEverythingFlag = false;}

	void markBubbleHaloChanged(BubbleId id);
	void markBubbleContentsChanged(BubbleId id);
	void clear() {
		myChangedBubbleHalos   .clear();
		myChangedBubbleContents.clear();
		myWindowViewportChanged.clear();
	};
};


