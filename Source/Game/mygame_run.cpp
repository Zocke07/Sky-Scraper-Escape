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
	if (current->isPause() == false)
	{
		current->OnMove();
	}
	if (current->isToInit() == true)
	{
		current->setToInit(false);
		GotoGameState(GAME_STATE_INIT);
	}
	if (current->isRetry() == true)
	{
		current->setRetry(false);
		GotoGameState(GAME_STATE_RUN);
	}
}

void CGameStateRun::OnInit()  								// Game initial values and graphics settings
{
	current->OnInit();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	current->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	current->OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // Handling mouse movements
{
	current->OnLButtonDown(nFlags, point);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// Handling mouse movements
{
	current->OnLButtonUp(nFlags, point);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// Handling mouse movements
{
	current->OnMouseMove(nFlags, point);
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // Handling mouse movements
{
	current->OnRButtonDown(nFlags, point);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// Handling mouse movements
{
	current->OnRButtonUp(nFlags, point);
}

void CGameStateRun::OnShow()
{
	current->OnShow();
	vector<levels::writeText> texts = current->getText();
	CDC* pDC = CDDraw::GetBackCDC();
	for(levels::writeText t: texts)
	{
		CTextDraw::ChangeFontLog(pDC, t.size, "Courier New", t.color,620);
		CTextDraw::Print(pDC, (int)t.position.x, (int)t.position.y, t.text);
	}
	CTextDraw::Print(pDC, 20, 90, "Level   : " + to_string(currentLevel));
	CDDraw::ReleaseBackCDC();
}
