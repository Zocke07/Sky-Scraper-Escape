#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <iostream>

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
	background.LoadBitmapByString({"Resources/background.bmp"});
	background.SetTopLeft(0, 0);
	//
	Sleep(1000);				// Slow down to see the progress clearly. Please delete this Sleep for the actual game
	//
	// This OnInit action will receive CGameStaterRun::OnInit(), so the progress has not reached 100%.
	//
}

void CGameStateInit::OnBeginState()
{
	GotoGameState(GAME_STATE_RUN);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();
}