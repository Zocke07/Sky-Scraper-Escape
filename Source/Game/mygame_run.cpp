#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <experimental/filesystem>
#include <string>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// This class is the game execution object of the game. The main game programs are all here.
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
	theLevel1.OnBeginState();
}

void CGameStateRun::OnMove()							// Moving game element
{
	if (currentLevel == 1)
	{
		if (theLevel1.isPause() == false)
		{
			theLevel1.OnMove();
		}
		if (theLevel1.getToInit() == true)
		{
			theLevel1.setToInit(false);
			GotoGameState(GAME_STATE_INIT);
		}
		if (theLevel1.isRetry() == true)
		{
			theLevel1.setRetry(false);
			GotoGameState(GAME_STATE_RUN);
		}
	}
}

void CGameStateRun::OnInit()  								// Game initial values and graphics settings
{
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theLevel1.OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // Handling mouse movements
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// Handling mouse movements
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// Handling mouse movements
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // Handling mouse movements
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// Handling mouse movements
{
}

void CGameStateRun::OnShow()
{
	theLevel1.OnShow();
	vector<levels::writeText> texts = theLevel1.getText();
	CDC* pDC = CDDraw::GetBackCDC();
	for(levels::writeText t: texts)
	{
		CTextDraw::ChangeFontLog(pDC, t.size, "Courier New", t.color,620);
		CTextDraw::Print(pDC, (int)t.position.x, (int)t.position.y, t.text);
	}
	CDDraw::ReleaseBackCDC();
}