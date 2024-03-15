//#define	 INITGUID
#include "stdafx.h"
#include "../Core/game.h"
#include "../Core/MainFrm.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gameutil.h"
#include "gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// CMovingBitmap: Moving Bitmap class
	// This class provides movable graphics
	// You need to know how to call (use) its various abilities, but you don’t have to understand what the following programs mean.
	/////////////////////////////////////////////////////////////////////////////

	//! CMovingBitmap constructor
	/*! 
		Used to create an object that has not yet read the image.
	*/
	CMovingBitmap::CMovingBitmap()
	{
		isBitmapLoaded = false;
	}

	//! Get the image height of the CMovingBitmap object.
	/*!
		Images need to be loaded first.
		\return Image height, in pixels.
	*/
	int CMovingBitmap::GetHeight()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Height() is called !!!");
		return locations[frameIndex].bottom - locations[frameIndex].top;
	}

	//! Get the x-axis coordinate value of the upper left corner of the CMovingBitmap object.
	/*!
		Images need to be loaded first.
		\return x-axis coordinate value of the upper left corner of the image.
	*/
	int CMovingBitmap::GetLeft()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Left() is called !!!");
		return locations[frameIndex].left;
	}

	//! Read image resources.
	/*!
		Read the corresponding image through the resource number `IDB_BITMAP` and filter the specific color `color`.
		\param IDB_BITMAP image resource number
		\param color The color to be filtered (default is `CLR_INVALID`, you can use `RGB(<R>, <G>, <B>`) to set the color)
	*/
	void CMovingBitmap::LoadBitmap(int IDB_BITMAP, COLORREF color)
	{
		CBitmap bitmap;
		BOOL rval = bitmap.LoadBitmap(IDB_BITMAP);
		GAME_ASSERT(rval, "Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
		BITMAP bitmapSize;
		bitmap.GetBitmap(&bitmapSize);
		
		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmap(IDB_BITMAP, color));
		filterColor = color;
		isBitmapLoaded = true;
	}

	//! Read image resources.
	/*!
		Read the corresponding image through the image relative path `filepath`, and filter the specific color `color`.
		\param filepath picture relative path
		\param color The color to be filtered (default is `CLR_INVALID`, you can use `RGB(<R>, <G>, <B>`) to set the filter color)
	*/
	void CMovingBitmap::LoadBitmap(char *filepath, COLORREF color)
	{
		HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hbitmap == NULL) {
			char error_msg[300];
			sprintf(error_msg, "Loading bitmap	from file \"%s\" failed !!!", filepath);
			GAME_ASSERT(false, error_msg);
		}

		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically
		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);

		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmap(filepath, color));
		imageFileName = string(filepath);
		filterColor = color;
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}

	//! Read image resources.
	/*!
		Read multiple images through the image relative path set `filepaths`, and filter specific colors `color`.
		\param filepaths picture relative path set
		\param color The color to be filtered (default is `CLR_INVALID`, you can use `RGB(<R>, <G>, <B>`) to set the filter color)
		\sa https://en.cppreference.com/w/cpp/container/vector
	*/
	void CMovingBitmap::LoadBitmap(vector<char*> filepaths, COLORREF color)
	{
		for (int i = 0; i < (int)filepaths.size(); i++) {
			LoadBitmap(filepaths[i], color);
		}
	}

	//! Read image resources.
	/*!
		Read multiple images through the image relative path set `filepaths`, and filter specific colors `color`.
		\param filepaths picture relative path set
		\param color The color to be filtered (default is `CLR_INVALID`, you can use `RGB(<R>, <G>, <B>`) to set the filter color)
		\sa https://en.cppreference.com/w/cpp/container/vector
	*/
	void CMovingBitmap::LoadBitmapByString(vector<string> filepaths, COLORREF color)
	{

		for (int i = 0; i < (int)filepaths.size(); i++) {
			LoadBitmap((char*)filepaths[i].c_str(), color);
		}
	}
	
	//! Read blank image resources.
	/*!
		Reads a white bitmap of a specific size.
		\param height image length
		\param width image width
	*/
	void CMovingBitmap::LoadEmptyBitmap(int height, int width) {
		HBITMAP hbitmap = CreateBitmap(width, height, 1, 32, NULL);
		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically

		/* Fill white color to bitmap */
		HDC hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hbitmap);
		PatBlt(hdc, 0, 0, width, height, WHITENESS);
		SelectObject(hdc, hOldBitmap);
		DeleteDC(hdc);

		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);

		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmapWithHBITMAP(hbitmap));
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}
	
	//! Stop showing images.
	/*!
		@deprecated is abandoned from version v1.0.0. To stop displaying images, please do not call `ShowBitmap()` during `OnShow()`.
		\sa ShowBitmap()
	*/
	void CMovingBitmap::UnshowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");
		isAnimation = false;
		this->ShowBitmap(0);
	}

	//! Set the image to the specified coordinates on the canvas.
	/*!
		The upper left corner of the image will be set to the specified coordinates.
		\param x Upper left corner x-coordinate
		\param y Upper left corner y-coordinate
	*/
	void CMovingBitmap::SetTopLeft(int x, int y)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");

		for (int i = 0; i < int(locations.size()); i++) {
			int dx = locations[i].left - x;
			int dy = locations[i].top - y;
			locations[i].left = x;
			locations[i].top = y;
			locations[i].right -= dx;
			locations[i].bottom -= dy;
		}
	}

	//! Set whether the picture is animated.
	/*!
		If CMovingBitmap reads multiple images, you can use this function to animate the object.
		\param delay animation switching delay (in milliseconds)
		\param once Whether the animation is a one-time animation, if so, you need to call `ToggleAnimation()` to start the animation.
		\sa ToggleAnimation()
	*/
	void CMovingBitmap::SetAnimation(int delay, bool once) {
		if(!once) isAnimation = true;
		isOnce = once;
		delayCount = delay;
	}
	
	//! Displays picture.
	/*!
		Can only be called during `onShow()` and the image needs to be read.
	*/
	void CMovingBitmap::ShowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(surfaceID[frameIndex], locations[frameIndex].left, locations[frameIndex].top);
		ShowBitmapBySetting();
	}

	//! Displays picture.
	/*!
		Can only be called during `onShow()` and the image needs to be read.
		\param factor Magnification, requires VGA graphics card support, otherwise it will become extremely slow.
	*/
	void CMovingBitmap::ShowBitmap(double factor)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(surfaceID[frameIndex], locations[frameIndex].left, locations[frameIndex].top, factor);
		ShowBitmapBySetting();
	}

	//! Set the index value of the current picture display frame.
	/*!
		Image display frame index values start with 0.
		\param frameIndex The index value of the picture display frame.
	*/
	void CMovingBitmap::SetFrameIndexOfBitmap(int frameIndex) {
		GAME_ASSERT(frameIndex < (int) surfaceID.size(), "選擇圖片時索引出界");
		this->frameIndex = frameIndex;
	}

	//! Get the index value of the current picture display frame.
	/*!
		\return The index value of the image display frame.
	*/
	int CMovingBitmap::GetFrameIndexOfBitmap() {
		return frameIndex;
	}

	//! Get the y-axis coordinate value of the upper left corner of the current picture.
	/*!
		\return The coordinate value of the y-axis of the upper left corner of the picture.
	*/
	int CMovingBitmap::GetTop()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Top() is called !!!");
		return locations[frameIndex].top;
	}

	//! Get the current image width.
	/*!
		\return Get the current image width.
	*/
	int CMovingBitmap::GetWidth()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Width() is called !!!");
		return locations[frameIndex].right - locations[frameIndex].left;
	}
	
	//! Start a single animation.
	/*!
		Set the animation to the initial frame and initialize the parameter values of the single animation.
	*/
	void CMovingBitmap::ToggleAnimation() {
		frameIndex = 0;
		isAnimation = true;
		isAnimationDone = false;
	}

	//! Whether the object is an animated object.
	/*!
		\return Boolean value, indicating whether the object is an animated object.
	*/
	bool CMovingBitmap::IsAnimation() {
		return isAnimation;
	}

	//! Whether the animation object has completed animation.
	/*!
		\return Boolean value, indicating whether the animation object has completed animation.
	*/
	bool CMovingBitmap::IsAnimationDone() {
		return isAnimationDone;
	}

	//! Whether the animated object is a one-shot animated object.
	/*!
		\return Boolean value, indicating whether the animated object is a one-shot animated object.
	*/
	bool CMovingBitmap::IsOnceAnimation() {
		return isOnce;
	}

	//! Whether the object has read the bitmap.
	/*!
	\return Boolean value, indicating whether the object has read the bitmap.
	*/
	bool CMovingBitmap::IsBitmapLoaded() {
		return isBitmapLoaded;
	}

	//! Returns the frame number of the object.
	/*!
		\return Returns the frame number of the object.
	*/
	int CMovingBitmap::GetFrameSizeOfBitmap() {
		return (int) surfaceID.size();
	}

	//! Initialize the location object in CMovingBitmap according to BITMAP.
	/*!
		\param bitmapSize Initialized BITMAP object with bitmap height and width
	*/
	void CMovingBitmap::InitializeRectByBITMAP(BITMAP bitmapSize) {
		const unsigned NX = 0;
		const unsigned NY = 0;
		CRect newCrect;
		newCrect.left = NX;
		newCrect.top = NY;
		newCrect.right = NX + bitmapSize.bmWidth;
		newCrect.bottom = NY + bitmapSize.bmHeight;
		locations.push_back(newCrect);
	}

	//! Display images according to parameters set by the user.
	void CMovingBitmap::ShowBitmapBySetting() {
		if (isAnimation == true && clock() - last_time >= delayCount) {
			frameIndex += 1;
			last_time = clock();
			if (frameIndex == surfaceID.size() && animationCount > 0) {
				animationCount -= 1;
			}
			if (frameIndex == surfaceID.size() && (isOnce || animationCount == 0)) {
				isAnimation = false;
				isAnimationDone = true;
				frameIndex = surfaceID.size() - 1;
				return;
			}
			frameIndex = frameIndex % surfaceID.size();
		}
	}

	//! Get the name of the object loading image.
	/*!
		\return Returns the image name. If the image has not been loaded, an empty string is returned.
	*/
	string CMovingBitmap::GetImageFileName() {
		return imageFileName;
	}

	//! Get the object filter color.
	/*!
		\return Returns the filter color. If the object does not have a filter color set, returns `CLR_INVALID`.
	*/
	COLORREF CMovingBitmap::GetFilterColor() {
		return filterColor;
	}

	//! Whether the two objects overlap.
	/*!
		\param bmp1 first CMovingBitmap object
		\param bmp2 second CMovingBitmap object
		\return Returns the Boolean value, indicating whether the two objects overlap.
	*/
	bool CMovingBitmap::IsOverlap(CMovingBitmap bmp1, CMovingBitmap bmp2) {
		CRect rect;
		BOOL isOverlap = rect.IntersectRect(bmp1.locations[bmp1.GetFrameIndexOfBitmap()], bmp2.locations[bmp2.GetFrameIndexOfBitmap()]);
		return isOverlap;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CTextDraw: The class provide the ability to draw the text.
	// This class provides text rendering
	// You need to know how to call (use) its various abilities, but you don’t have to understand what the following programs mean.
	/////////////////////////////////////////////////////////////////////////////

	//! Print text on the screen.
	/*!
	We hand over the responsibility of pDC acquisition to the caller. You must obtain the current pDC pointer and then remember to release it.
		\param pDC, a CDC indicator, obtainable using `CDDraw::GetBackCDC()`.
		\param x x coordinate of the upper left corner of the text display
		\param y y coordinate of the upper left corner of the text display
		\param str text to be displayed
	*/
	void CTextDraw::Print(CDC *pDC, int x, int y, string str) {
		x = CDDraw::IsFullScreen() ? x + (RESOLUTION_X - SIZE_X) / 2 : x;
		y = CDDraw::IsFullScreen() ? y + (RESOLUTION_Y - SIZE_Y) / 2 : y;
		pDC->TextOut(x, y, str.c_str());
	}

	//! Set the properties of the current text.
	/*!
	We hand over the responsibility of pDC acquisition to the caller. You must obtain the current pDC pointer and then remember to release it.
	This setting will lose its set value when the set pDC pointer is released.
		\param pDC，A CDC indicator, obtainable using `CDDraw::GetBackCDC()`.
		\param size The size of the text (in pt, but due to limited technology, it may not exactly match the size in pt)
		\param fontName Font name
		\param fontColor Font color
		\param weight Font thickness (default is 500)
	*/
	void CTextDraw::ChangeFontLog(CDC *pDC, int size, string fontName, COLORREF fontColor, int weight) {
		CFont* fp;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontColor);
		LOGFONT lf;
		CFont f;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = -MulDiv(size, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 96);
		lf.lfWeight = weight;
		strcpy(lf.lfFaceName, fontName.c_str());
		f.CreateFontIndirect(&lf);
		fp = pDC->SelectObject(&f);
	}

}         
