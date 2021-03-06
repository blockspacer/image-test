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

	void redrawAllWindows() {
		myRedrawEverythingFlag = true;
		#ifdef WEB
			emscripten_resume_main_loop();
		#endif
	};
	bool doRedrawEverything() {return myRedrawEverythingFlag;}
	void haveRedrawnEverything() {myRedrawEverythingFlag = false;}

	void markBubbleHaloChanged(BubbleId id);
	void markBubbleContentsChanged(BubbleId id);
	void clear() {
		myChangedBubbleHalos   .clear();
		myChangedBubbleContents.clear();
		myWindowViewportChanged.clear();
	};

	void redrawPanningBar(WindowId winId) {
		// todo: only redraw this one window's panning bar
		redrawAllWindows();
	};
};


/*

a single window's panning bar (when the user mouses over it)
all windows' panning bars (when bubbles or viewports move or change size)

all windows containing a view of a bubble
all windows containing a view of a bubble group

*/