#pragma once
/*
 * gamelib.h: This file stores the interface of game-related classes
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *		1. Add ENABLE_AUDIO flag.
 *   2004-03-02 V4.0
 *		1. Add CInteger class.
 *      2. Add CGameState abstract class and adjust CGame to apply
 *         state pattern for switching game states.
 *   2004-03-08 V4.1
 *      1. Add OPEN_AS_FULLSCREEN flag.
 *      2. The Game Engine now becomes a framework.
 *   2005-07-28
 *      1. Add GAME_ASSERT macro to display run time errors gracefully.
 *      2. Remove CDDarw::CheckSystem(). It is useless.
 *   2005-09-08
 *      1. Eliminate CSpecialEffect::Abort. It is useless now.
 *      2. Add SHOW_GAME_CYCLE_TIME flag.
 *      3. Add DEFAULT_BG_COLOR flag;
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add MOUSEMOVE Handler for CGame and CGameState.
 *      2. Add _TRACE preprocessor flag for VC++.net.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Remove constantness of CAnimation::DELAY_COUNT.
 *      3. Enhance CAnimation to support SetDelayCount(), Reset(), and IsFinalBitmap().
 *      4. Remove CAnimation::GetLocation() and CMovingBitmap::GetLocation().
 *      5. Bitmap coordinate can no longer be set by CMovingBitmap::LoadBitmap().
 *         Defauts to (0,0).
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *      2. Fix bug: audio is now correctly recovered after a sleep or suspension of windows.
 *      3. Support ENABLE_GAME_PAUSE.
 *   2008-02-15 V4.4
 *      1. Add setup project for Visual studio 2005.
 *      2. Support bitmap scaling when ShowBitmap(scale) is called.
 *      3. Add namespace game_framework.
 *      4. Make the class CGame a singleton so that MFC can access it easily.
 *      5. Support loading of bitmap from bmp file.
 *      6. Support ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Rewrite CAudio with MCI commands to eliminate dependency with DirectMusic.
*/

/////////////////////////////////////////////////////////////////////////////
// Header for STL (Standard Template Library)
/////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>
#include <map>
using namespace std;

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// This class provides dynamic (movable) graphics
	// You must understand the usage of each Public Interface, but you don¡¦t need to understand Implementation.
	/////////////////////////////////////////////////////////////////////////////

	class CMovingBitmap {
	public:
		CMovingBitmap();

		/* The function for loading the bitmap. */
		void  LoadBitmap(int, COLORREF = CLR_INVALID);		// Load the image and specify the image number (resource) and transparent color
		void  LoadBitmap(char*, COLORREF = CLR_INVALID);	// Load the image, specify the file name and transparent color of the image
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);	// Load the image, specify the file name and transparent color of the image
		void  LoadBitmapByString(vector<string>, COLORREF = CLR_INVALID);	// Load the image, specify the file name and transparent color of the image
		void  LoadEmptyBitmap(int height, int weight);
		
		/* Unshow the bitmap. */
		void  UnshowBitmap();

		/* Setter */
		void  SetAnimation(int delay, bool _once);
		void  SetFrameIndexOfBitmap(int frame);
		void  SetTopLeft(int, int);			// Move the coordinates of the upper left corner of the figure to (x,y)

		/* Show the bitmap with or without factor. */
		void  ShowBitmap();					// Paste image to screen
		void  ShowBitmap(double factor);	// Paste the image to the screen. When factor < 1, reduce the size, and when > 1, zoom in. Note: VGA card hardware support is required, otherwise it will be very slow.
		
		/* Getter */
		int   GetFrameIndexOfBitmap();
		int   GetFrameSizeOfBitmap();
		int   GetTop();
		int   GetLeft();
		int   GetHeight();
		int   GetWidth();
		string GetImageFileName();
		COLORREF GetFilterColor();

		/* Is function */
		bool  IsAnimation();
		bool  IsAnimationDone();
		bool  IsBitmapLoaded();
		bool  IsOnceAnimation();
		static bool IsOverlap(CMovingBitmap bmp1, CMovingBitmap bmp2);
		
		/* Toggle function */
		void  ToggleAnimation();

	protected:
		//! The index value of the current frame.
		int frameIndex = 0;
		//! The delay in switching the current frame.
		int delayCount = 10;
		//! Store the current animation times.
		int animationCount = -1;
		//! Whether the stored object is animated.
		bool isAnimation = false;
		//! Stores whether the object animation has ended
		bool isAnimationDone = true;
		//! Whether the stored image has been read
		bool isBitmapLoaded = false;	// whether a bitmap has been loaded
		//! Whether the stored object animation is a one-shot animation
		bool isOnce = false;
		vector<CRect>    locations;			// location of the bitmap
		vector<unsigned> surfaceID;
		clock_t last_time = clock();
		//! The image path to be read by the storage object
		string   imageFileName = "";
		//! Store the image color of the object filter
		COLORREF filterColor = CLR_INVALID;

	private:
		void InitializeRectByBITMAP(BITMAP bitmap);
		void ShowBitmapBySetting();
	};

	class CTextDraw {
	public:
		void static Print(CDC *pdc, int x, int y, string str);
		void static ChangeFontLog(CDC *pdc, int size, string fontName, COLORREF fontColor, int weight = 500);
	};

}
