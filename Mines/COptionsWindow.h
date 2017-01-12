//##############################################################################
// Mines - CMinesApp.h				by Matt Gallagher �1999 All rights reserved
//##############################################################################

#pragma once

#include <LWindow.h>

// Message for directing the window to start a new game
typedef struct NewGameOptionsMsg {
	Int16	fieldSize;
	Int16	numMines, gameLength;
	Boolean	autoStart, untimed;
	short*	fieldData;
} NewGameOptionsMsg;

class COptionsWindow : public LWindow, public LListener {
public:
	enum { class_ID = 'Owin' };
	
	// Constructor and destructor (both empty implementations)
			COptionsWindow(LStream* inStream);
	virtual	~COptionsWindow();

	// Overridden inherited commands
	void	FinishCreateSelf();
	Boolean	ObeyCommand( CommandT inCommand, void *ioParam );
	void	ListenToMessage( MessageT inMessage, void *ioParam );

private:
	// Private methods
	void	SwitchTabPane( Int32 value );

	// Private variables
	LView	*mCurrentPanel;
	
	// Private Configuration options
	Boolean	mAutoStart, mUntimed, mCustomDifficulty;
	Int16	mNumMines, mGameSize, mGameTime, mDifficultyLevel;
	Int16	mCurrentSize, mCurrentTime;
};
