//##############################################################################
// Mines - CMinesWindow.h			by Matt Gallagher �1999 All rights reserved
//##############################################################################

#include "COptionsWindow.h"
#include "CMinesConstants.h"

// MacOS header for MathLib pow()
#include <fp.h>

// Bunch of different controls
#include <LTabsControl.h>
#include <LCheckBoxGroupBox.h>
#include <LPopupGroupBox.h>
#include <LStaticText.h>
#include <LPopupButton.h>
#include <LLittleArrows.h>
#include <LSlider.h>
#include <LRadioButton.h>

//#############################
// The constructor and the destructor
//#############################
COptionsWindow::COptionsWindow(LStream* inStream) : LWindow(inStream)
{
	// Clear the current panel so that it is free to be set
	mCurrentPanel = nil;
	
	// Initialise the new game options
	mAutoStart = false;
	mUntimed = false;
	mCustomDifficulty = false;
	mDifficultyLevel = 2;
	mNumMines = 2;
	mGameSize = 20;
	mGameTime = 200;
	mCurrentTime = 200;
	mCurrentSize = 20;
}
COptionsWindow::~COptionsWindow(){}

//#############################
// Finish create self to install the correct tab pane
//#############################
void COptionsWindow::FinishCreateSelf()
{
	// Call the normal window FinishCreateSelf()
	LWindow::FinishCreateSelf();
	
	// Find the window's screen rectangle
	Rect	screenRect;
	::GetRegionBounds(::GetGrayRgn(), &screenRect);
	Rect	windowRect = UWindows::GetWindowStructureRect( mMacWindowP );
	
	MoveWindowTo( screenRect.right - (windowRect.right - windowRect.left),
					screenRect.top + 20 );
	
	// Become a listener to all the window's controls
	UReanimator::LinkListenerToControls( this, this, rPPob_OptionsWindow );
	
	// Since LTabsControl is a LControlView, not a LControl, we must
	// listen to it separately.
	LTabsControl*	tabPanel =  (LTabsControl*)FindPaneByID ( kOptionsTabPane );
	if ( tabPanel ) { tabPanel->AddListener ( this ); }
	
	// Display the contents of the tab region to the first panel
	SInt32 tabValue = 1;
	ListenToMessage( msg_TabSwitch, &tabValue );
}

//#############################
// Listen to the controls in this window
//#############################
void COptionsWindow::SwitchTabPane( SInt32 value )
{
	LPlaceHolder* panelHost = dynamic_cast<LPlaceHolder *>
									(FindPaneByID(kOptionsPlaceHolder));
	
	if( panelHost )
	{
		LView *installedPanel;
		
		// Load the appropriate view from its PPob resource
		if( value == 1 )
			installedPanel = UReanimator::CreateView(
								rPPobDifficultyPane, panelHost, this );
		else
			installedPanel = UReanimator::CreateView(
								rPPobOtherOptionsPane, panelHost, this );
		ThrowIfNil_( installedPanel );
		
		// Install the view into the placeholder
		panelHost->RemoveOccupant();
		panelHost->InstallOccupant( installedPanel, kAlignNone );
		
		// Delete the previous contents of the panel
		if( mCurrentPanel )
		{
			mCurrentPanel->Hide();
			delete mCurrentPanel;
			mCurrentPanel = nil;
		}
		mCurrentPanel = installedPanel;
		
		if( value == 1 )
		{
			// Become a listener to all the controls
			UReanimator::LinkListenerToControls( this, installedPanel,
				rPPobDifficultyPane );
			
			// Adjust all the items in this pane
			LRadioButton	*difficultyButton, *customButton;
			LPopupButton	*difficultyMenu;
			LStaticText		*gameSizeTitle, *gameSize, *gameTimeTitle,
							*gameTime, *currentGameSize, *currentGameTime;
			LSlider			*gameSizeSlider;
			LLittleArrows	*gameTimeButtons;

			// Get all the panes in the window that we must deal with
			difficultyButton= (LRadioButton*)FindPaneByID(kDifficultyLevel);
			customButton	= (LRadioButton*)FindPaneByID(kCustomSize);
			difficultyMenu	= (LPopupButton*)FindPaneByID(kDifficultyPopup);
			gameSizeTitle	= (LStaticText*)FindPaneByID(kCustomSizeTitle);
			gameSize		= (LStaticText*)FindPaneByID(kCustomSizeText);
			gameTimeTitle	= (LStaticText*)FindPaneByID(kCustomGameTimeTitle);
			gameTime		= (LStaticText*)FindPaneByID(kCustomGameTimeText);
			currentGameSize	= (LStaticText*)FindPaneByID(kCurrentGameSize);
			currentGameTime	= (LStaticText*)FindPaneByID(kCurrentGameTime);
			gameSizeSlider	= (LSlider*)FindPaneByID(kCustomSizeSlider);
			gameTimeButtons	= (LLittleArrows*)FindPaneByID(kCustomGameTime);
			
			// Create the text strings we need
			LStr255	timeText = mGameTime;
					timeText = timeText + "\p seconds";
			LStr255 sizeText = mGameSize;
					sizeText = sizeText + "\px";
			LStr255 sizeText2= (long)(mGameSize*12/20);
					sizeText = sizeText + sizeText2;
					sizeText = sizeText + "\p squares";
			LStr255	customTimeText = mCurrentTime;
					customTimeText = customTimeText + "\p seconds";
			LStr255 customSizeText = mCurrentSize;
					customSizeText = customSizeText + "\px";
			LStr255 customSizeText2= (long)(mCurrentSize*12/20);
					customSizeText = customSizeText + customSizeText2;
					customSizeText = customSizeText + "\p squares";
			
			if( mCustomDifficulty == false )
			{
				// Enable the standard difficulty levels
				difficultyButton->SetValue( Button_On );
				difficultyMenu->Enable();

				// Disable the custom difficulties
				customButton->SetValue( Button_Off );
				gameSizeTitle->Disable();
				gameSize->Disable();
				gameSizeSlider->Disable();
				gameTimeTitle->Disable();
				gameTime->Disable();
				gameTimeButtons->Disable();
			}
			else
			{
				// Disable the standard difficulties
				difficultyMenu->SetValue( Button_Off );
				difficultyMenu->Disable();
				
				// Enable the custom difficulties
				customButton->SetValue( Button_On );
				gameSizeTitle->Enable();
				gameSize->Enable();
				gameSizeSlider->Enable();
				gameTimeTitle->Enable();
				gameTime->Enable();
				gameTimeButtons->Enable();
			}
			
			// Set all the text and control values correctly
			difficultyMenu->SetValue( mDifficultyLevel );
			gameSizeSlider->SetValue( mGameSize );
			gameTimeButtons->SetValue( mGameTime );
			gameSize->SetText( sizeText );
			gameTime->SetText( timeText );
			currentGameSize->SetText( customSizeText );
			currentGameTime->SetText( customTimeText );
		}
		else
		{
			// Become a listener to the ControlViews
			LCheckBoxGroupBox	*autoStartBox, *untimedBox;
			LPopupGroupBox		*numMinesBox;
			autoStartBox = (LCheckBoxGroupBox*)FindPaneByID(kAutoStartBox);
			autoStartBox->AddListener( this );
			untimedBox = (LCheckBoxGroupBox*)FindPaneByID(kUntimedBox);
			untimedBox->AddListener( this );
			numMinesBox = (LPopupGroupBox*)FindPaneByID(kNumMinesBox);
			numMinesBox->AddListener( this );
			
			// Set all the checkbox values to what they should be
			autoStartBox->SetValue( mAutoStart );
			untimedBox->SetValue( mUntimed );
			
			// Set the popup menu appropriately
			SInt16	menuItem = mNumMines;
			if( menuItem > 3 ) menuItem = 3;
			numMinesBox->SetValue( menuItem );
			
			// Set the value of the text box correctly
			SInt16	numMines = mGameSize * mNumMines / 2;
			LStr255	text = numMines;
			text = text + "\p mines";
			LStaticText *numMinesText = dynamic_cast<LStaticText*>
				(FindPaneByID(kNumMinesText));
			numMinesText->SetText( text );
		}
		
		// Refresh the pane
		panelHost->Refresh();
	}
}

//#############################
// Listen to commands from MinesWindows
//#############################
Boolean COptionsWindow::ObeyCommand( CommandT inCommand, void *ioParam )
{
	Boolean cmdHandled=true;

	switch( inCommand )
	{
		case cmd_NewFrontWindow:
		{
			NewGameOptionsMsg	*inMessage = (NewGameOptionsMsg*)ioParam;
			
			mCurrentSize = inMessage->fieldSize;
			mCurrentTime = inMessage->gameLength;
			
			LTabsControl *tabsControl = dynamic_cast<LTabsControl*>
				(FindPaneByID(kOptionsTabPane));
			if( tabsControl->GetValue() == 1 )
			{
				LStaticText *currentGameSize =
					(LStaticText*)FindPaneByID(kCurrentGameSize);
				LStaticText *currentGameTime =
					(LStaticText*)FindPaneByID(kCurrentGameTime);
				LStr255	customTimeText = mCurrentTime;
						customTimeText = customTimeText + "\p seconds";
				LStr255 customSizeText = mCurrentSize;
						customSizeText = customSizeText + "\px";
				LStr255 customSizeText2= (long)(mCurrentSize*12/20);
						customSizeText = customSizeText + customSizeText2;
						customSizeText = customSizeText + "\p squares";
				currentGameSize->SetText( customSizeText );
				currentGameTime->SetText( customTimeText );
			}
		}
		break;
		
		default:
		{
			// Call inherited.
			cmdHandled = LWindow::ObeyCommand( inCommand, ioParam );
		}
		break;
	}
	
	return cmdHandled;
}

//#############################
// Listen to the controls in this window
//#############################
void COptionsWindow::ListenToMessage( MessageT inMessage, void *ioParam )
{
	SInt32 value;
	
	switch( inMessage )
	{
		case msg_TabSwitch:
		{
			value = *(SInt32 *) ioParam;
			SwitchTabPane( value );
		}
		break;
		
		case msg_NewGameButton:
		{
			NewGameOptionsMsg	outMessage;
			
			// Fill in the message to send
			outMessage.fieldSize = mGameSize;
			outMessage.numMines = mGameSize * mNumMines / 2;
			outMessage.gameLength = mGameTime;
			outMessage.autoStart = mAutoStart;
			outMessage.untimed = mUntimed;
			outMessage.fieldData = nil;
			
			// Send the message
			LCommander	*theTarget = LCommander::GetTarget();
			theTarget->ProcessCommand( cmd_NewCustomGame, &outMessage );
		}
		break;
		
		case msg_AutoStart:
		{
			value = *(SInt32 *) ioParam;
			mAutoStart = value;
		}
		break;
		
		case msg_Untimed:
		{
			value = *(SInt32 *) ioParam;
			mUntimed = value;
		}
		break;
		
		case msg_NumMines:
		{
			value = *(SInt32 *) ioParam;
			mNumMines = value;
			if( mNumMines == 3 ) mNumMines = 8;
			
			// Set the value of the text box correctly
			SInt16	numMines = mGameSize * mNumMines / 2;
			LStr255	text = numMines;
			text = text + "\p mines";
			LStaticText *numMinesText = dynamic_cast<LStaticText*>
				(FindPaneByID(kNumMinesText));
			numMinesText->SetText( text );
		}
		break;
		
		case msg_DifficultyLevel:
		{
			value = *(SInt32 *) ioParam;
			if( value == Button_On )
			{
				mCustomDifficulty = false;
				
				// Adjust all the items in this pane
				LPopupButton	*difficultyMenu;
				LStaticText		*gameSizeTitle, *gameSize, *gameTimeTitle,
								*gameTime;
				LSlider			*gameSizeSlider;
				LLittleArrows	*gameTimeButtons;

				// Get all the panes in the window that we must deal with
				difficultyMenu	= (LPopupButton*)FindPaneByID(kDifficultyPopup);
				gameSizeTitle	= (LStaticText*)FindPaneByID(kCustomSizeTitle);
				gameSize		= (LStaticText*)FindPaneByID(kCustomSizeText);
				gameTimeTitle	= (LStaticText*)FindPaneByID(kCustomGameTimeTitle);
				gameTime		= (LStaticText*)FindPaneByID(kCustomGameTimeText);
				gameSizeSlider	= (LSlider*)FindPaneByID(kCustomSizeSlider);
				gameTimeButtons	= (LLittleArrows*)FindPaneByID(kCustomGameTime);
				
				// Enable the standard difficulty levels
				difficultyMenu->Enable();

				// Disable the custom difficulties
				gameSizeTitle->Disable();
				gameSize->Disable();
				gameSizeSlider->Disable();
				gameTimeTitle->Disable();
				gameTime->Disable();
				gameTimeButtons->Disable();
				
				// Send a message to change to the selected difficulty
				SInt32	level = mDifficultyLevel;
				ListenToMessage( msg_Difficulty, &level );
			}
		}
		break;
		
		case msg_Difficulty:
		{
			if( mCustomDifficulty == false )
			{
				value = *(SInt32 *) ioParam;

				// Recalculate the game dimensions for this new level
				mDifficultyLevel = value;
				mGameSize = ::pow( 2, value-1 ) * 10; // 10, 20 or 40
				mGameTime = ::pow( 2, 3-value ) * 100; // 400, 200, or 100
				mNumMines = value;
				if( mNumMines == 3 ) mNumMines = 8;
				
				// Adjust the slider and the little buttons
				LSlider			*gameSizeSlider;
				LLittleArrows	*gameTimeButtons;
				gameSizeSlider	= (LSlider*)FindPaneByID(kCustomSizeSlider);
				gameTimeButtons	= (LLittleArrows*)FindPaneByID(kCustomGameTime);
				gameSizeSlider->SetValue( mGameSize );
				gameTimeButtons->SetValue( mGameTime );
				
				// Get the text displays
				LStaticText *gameSize, *gameTime;
				gameSize		= (LStaticText*)FindPaneByID(kCustomSizeText);
				gameTime		= (LStaticText*)FindPaneByID(kCustomGameTimeText);
				
				// Create the text strings we need
				LStr255	timeText = mGameTime;
						timeText = timeText + "\p seconds";
				LStr255 sizeText = mGameSize;
						sizeText = sizeText + "\px";
				LStr255 sizeText2= (long)(mGameSize*12/20);
						sizeText = sizeText + sizeText2;
						sizeText = sizeText + "\p squares";
				
				// Display the new sizes
				gameSize->SetText( sizeText );
				gameTime->SetText( timeText );
			}
		}
		break;
		
		case msg_Custom:
		{
			mCustomDifficulty = true;
			
			// Adjust all the items in this pane
			LPopupButton	*difficultyMenu;
			LStaticText		*gameSizeTitle, *gameSize, *gameTimeTitle,
							*gameTime;
			LSlider			*gameSizeSlider;
			LLittleArrows	*gameTimeButtons;

			// Get all the panes in the window that we must deal with
			difficultyMenu	= (LPopupButton*)FindPaneByID(kDifficultyPopup);
			gameSizeTitle	= (LStaticText*)FindPaneByID(kCustomSizeTitle);
			gameSize		= (LStaticText*)FindPaneByID(kCustomSizeText);
			gameTimeTitle	= (LStaticText*)FindPaneByID(kCustomGameTimeTitle);
			gameTime		= (LStaticText*)FindPaneByID(kCustomGameTimeText);
			gameSizeSlider	= (LSlider*)FindPaneByID(kCustomSizeSlider);
			gameTimeButtons	= (LLittleArrows*)FindPaneByID(kCustomGameTime);
			
			// Enable the standard difficulty levels
			difficultyMenu->Disable();

			// Disable the custom difficulties
			gameSizeTitle->Enable();
			gameSize->Enable();
			gameSizeSlider->Enable();
			gameTimeTitle->Enable();
			gameTime->Enable();
			gameTimeButtons->Enable();
		}
		break;
		
		case msg_GameSize:
		{
			value = *(SInt32 *) ioParam;
			mGameSize = value;

			// Get the text displays
			LStaticText *gameSize;
			gameSize		= (LStaticText*)FindPaneByID(kCustomSizeText);
			
			// Create the text strings we need
			LStr255 sizeText = mGameSize;
					sizeText = sizeText + "\px";
			LStr255 sizeText2= (long)(mGameSize*12/20);
					sizeText = sizeText + sizeText2;
					sizeText = sizeText + "\p squares";
			
			// Display the new sizes
			gameSize->SetText( sizeText );

		}
		break;
		
		case msg_GameTime:
		{
			value = *(SInt32 *) ioParam;
			mGameTime = value;

			// Get the text displays
			LStaticText *gameTime;
			gameTime		= (LStaticText*)FindPaneByID(kCustomGameTimeText);
			
			// Create the text strings we need
			LStr255	timeText = mGameTime;
					timeText = timeText + "\p seconds";
			
			// Display the new sizes
			gameTime->SetText( timeText );
		}
		break;
		
		default:
		{
			// There is no appropriate default behaviour
		}
		break;
	}
}
