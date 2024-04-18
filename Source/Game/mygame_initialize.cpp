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
	if (nChar == VK_RETURN)
	{
		phase = 2;
	}

	if (phase == 2)
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
		if (selectArrow.GetTop() == 450 && nChar == VK_SPACE)
		{
			phase = 1;
			selector = 1;
			GotoGameState(GAME_STATE_RUN);
		}
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();

	if (phase == 1)
	{
		draw_text1();
	}
	else if (phase == 2)
	{
		draw_text2();
		selectArrow.ShowBitmap();
	}
}

void CGameStateInit::load_background()
{
	background.LoadBitmapByString({"Resources/background.bmp"});
	background.SetTopLeft(0, 0);
}

void CGameStateInit::load_icons()
{
	selectArrow.LoadBitmapByString({"Resources/SelectionArrow.bmp"}, RGB(0, 100, 0));
	selectArrow.SetTopLeft(400, 450);
}


void CGameStateInit::draw_text1()
{
	CDC *pDC = CDDraw::GetBackCDC();

	/* Print phase 1 */
	CTextDraw::ChangeFontLog(pDC, 36, "Arial", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 360, 380, "Press enter key to continue");

	CDDraw::ReleaseBackCDC();
}

void CGameStateInit::draw_text2()
{
	CDC *pDC = CDDraw::GetBackCDC();

	/* Print phase 2*/
	CTextDraw::ChangeFontLog(pDC, 24, "Arial", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 320, 400, "Use arrow up, arrow down, and space to select");
	CTextDraw::ChangeFontLog(pDC, 36, "Arial", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 500, 448, "Play");
	CTextDraw::Print(pDC, 500, 488, "Select Level");

	CDDraw::ReleaseBackCDC();
}
