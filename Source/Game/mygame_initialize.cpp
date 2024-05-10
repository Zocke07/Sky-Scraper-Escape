#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// This class is the game opening screen object of the game.
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// When there are many pictures, OnInit will take a lot of time to load all the pictures. In order
	//     to prevent gamers from waiting impatiently, the game will display "Loading..." to show the progress of Loading.
	//
	ShowInitProgress(0, "Start Initialize...");	// The initial loading progress is 0%
	//
	// Start loading data
	load_background();
	load_icons();
	//
	Sleep(1000);				// Slow down to see the progress clearly. Please delete this Sleep for the actual game
	//
	// This OnInit action will receive CGameStaterRun::OnInit(), so the progress has not reached 100%.
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN)
	{
		if (selector < 2)
		{
			selector += 1;
			selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() + 40);
		}
	}
	else if (nChar == VK_UP) {
		if (selector > 1) {
			selector -= 1;
			selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() - 40);
		}
	}
	// For now, only play button works
	if (selectArrow.GetTop() == 450 && nChar == VK_SPACE) // Play Button
	{
		GotoGameState(GAME_STATE_RUN);
	}
	else if (selectArrow.GetTop() == 450 && nChar == VK_SPACE) // Select Level Button
	{
		
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();
	title.ShowBitmap();
	draw_text();
	selectArrow.ShowBitmap();
}

void CGameStateInit::load_background()
{
	background.LoadBitmapByString({"Resources/background.bmp"});
	background.SetTopLeft(0, 0);
}

void CGameStateInit::load_icons()
{
	title.LoadBitmapByString({"Resources/Title.bmp"}, RGB(0, 100, 0));
	title.SetTopLeft(356, 200);
	selectArrow.LoadBitmapByString({"Resources/SelectionArrow.bmp"}, RGB(0, 100, 0));
	selectArrow.SetTopLeft(400, 450);
}

void CGameStateInit::draw_text()
{
	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 24, "Arial", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 340, 400, "Use arrow up, arrow down, and space to select");
	CTextDraw::ChangeFontLog(pDC, 36, "Arial", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 500, 448, "Play");
	CTextDraw::Print(pDC, 500, 488, "Select Level");

	CDDraw::ReleaseBackCDC();
}
