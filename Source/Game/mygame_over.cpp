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
// This class is the end state of the game (Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g): CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
}

void CGameStateOver::OnInit()
{
	//
	// When there are many pictures, OnInit will take a lot of time to load all the pictures. In order
	//     to prevent gamers from waiting impatiently, the game will display "Loading..." to show the progress of Loading.
	//
	ShowInitProgress(66, "Initialize...");	// Continuing the progress of the previous state, the progress here is regarded as 66%
	//
	// 開始載入資料
	//
	Sleep(1000);				// Slow down to see the progress clearly. Please delete this Sleep for the actual game
	//
	// Final progress is 100%
	//
	ShowInitProgress(100, "OK!");

	Sleep(1000);
}

void CGameStateOver::OnShow()
{

}
