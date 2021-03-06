//##############################################################################
// Mines - CMinesApp.h				by Matt Gallagher �1999 All rights reserved
//##############################################################################

#include "CMinesView.h"

//#############################
// Constructor
//#############################
CMinesView::CMinesView()
{
	mMineField = nil;
	if( mPictures.GetCount() == 0 ) LoadPictures();
}

//#############################
// Constructor
//#############################
CMinesView::CMinesView(const SPaneInfo &inPaneInfo,
					   const SViewInfo &inViewInfo)
	: LOffscreenView( inPaneInfo, inViewInfo )
{
	mMineField = nil;
	if( mPictures.GetCount() == 0 ) LoadPictures();
}

//#############################
// Constructor
//#############################
CMinesView::CMinesView(LStream* inStream) : LOffscreenView( inStream )
{
	mMineField = nil;
	if( mPictures.GetCount() == 0 ) LoadPictures();
}

//#############################
// Destructor
//#############################
CMinesView::~CMinesView()
{
}

//#############################
// Load all the pictures from the resources
//#############################
void CMinesView::LoadPictures()
{
	PicHandle	thePicture;
	
	thePicture = GetPicture( rZeroMines );
	mPictures.InsertItemsAt(1, kZeroMines, &thePicture);
	thePicture = GetPicture( rOneMines );
	mPictures.InsertItemsAt(1, kOneMines, &thePicture);
	thePicture = GetPicture( rTwoMines );
	mPictures.InsertItemsAt(1, kTwoMines, &thePicture);
	thePicture = GetPicture( rThreeMines );
	mPictures.InsertItemsAt(1, kThreeMines, &thePicture);
	thePicture = GetPicture( rFourMines );
	mPictures.InsertItemsAt(1, kFourMines, &thePicture);
	thePicture = GetPicture( rFiveMines );
	mPictures.InsertItemsAt(1, kFiveMines, &thePicture);
	thePicture = GetPicture( rSixMines );
	mPictures.InsertItemsAt(1, kSixMines, &thePicture);
	thePicture = GetPicture( rSevenMines );
	mPictures.InsertItemsAt(1, kSevenMines, &thePicture);
	thePicture = GetPicture( rEightMines );
	mPictures.InsertItemsAt(1, kEightMines, &thePicture);
	thePicture = GetPicture( rUntouchedSquare );
	mPictures.InsertItemsAt(1, kUntouchedSquare, &thePicture);
	thePicture = GetPicture( rRevealedBomb );
	mPictures.InsertItemsAt(1, kRevealedBomb, &thePicture);
	thePicture = GetPicture( rProtectedSquare );
	mPictures.InsertItemsAt(1, kProtectedSquare, &thePicture);
	thePicture = GetPicture( rPressedSquare );
	mPictures.InsertItemsAt(1, kPressedSquare, &thePicture);
}

//#############################
// Link to the mine field
//#############################
void CMinesView::LinkToField(CMinesWindow *mineField)
{
	mMineField = mineField;
	
	SDimension32 imageSize;
	mMineField->GetFieldSize( imageSize );
	mImageSize.height = imageSize.height;
	mImageSize.width = imageSize.width;
	
	mMineField->SetMaxWindowBounds(	mImageSize.width + 46,
									mImageSize.height + 128 );
}

//#############################
// Respond to a normal "update" event
//#############################
void CMinesView::DrawSelf()
{
	short	i, picID;
	Rect	picRect;
	Rect	localFrameRect;
	
	if( mMineField == nil ) return;
	
	CalcLocalFrameRect( localFrameRect );
	
	i=0;
	while( mMineField->GetSquare(i, picID, picRect) )
	{
		PicHandle	thePicture;
		Rect		tempRect;
		
		if( ::SectRect( &picRect, &localFrameRect, &tempRect ) )
		{
			mPictures.FetchItemAt(picID, &thePicture);
			::DrawPicture( thePicture, &picRect );
		}
		i++;
	}
}

//#############################
// Draw an individual square direct to screen on request
//#############################
void CMinesView::DrawSquare( short picID, Rect &picRect )
{
	FocusDraw();
	PicHandle thePicture;
	mPictures.FetchItemAt(picID, &thePicture);
	::DrawPicture( thePicture, &picRect );
}

//#############################
// Offscreen views are normally transparent to clicks. Revert to LView behavior.
//#############################
void CMinesView::Click(SMouseDownEvent &inMouseDown)
{
	if (!inMouseDown.delaySelect) {
			// Until now, whereLocal is actually in port coords. Now
			// that we know what Pane is handling the click, we can
			// convert it to the proper local coords.
		PortToLocalPoint(inMouseDown.whereLocal);
		UpdateClickCount(inMouseDown);
			// A ControlSubPane is an implementation detail.
			// Logically, we should executed the click attachments of
			// the LControlView containing this pane.
		if (mSuperView->ExecuteAttachments(msg_Click, &inMouseDown)) {
			ClickSelf(inMouseDown);
		}
	}
}

//#############################
// Handle a click in the minefield
//#############################
void CMinesView::ClickSelf(const SMouseDownEvent &mouseEvent)
{
	if( mMineField == nil ) return;

	mMineField->TrackClick( mouseEvent.whereLocal.h,
							mouseEvent.whereLocal.v,
							mouseEvent.macEvent.modifiers);
}
