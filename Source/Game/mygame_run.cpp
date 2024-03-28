#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <experimental/filesystem>

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
}

void CGameStateRun::OnMove()							// Moving game element
{
	int x = character.getX();
	int y = character.getYactual();
	int velocityY = character.getVelocityY();			// 正往下, 負往上
	int velocityX = character.getVelocityX();

	/*if (velocityY > 0) {	// Y速度向下
		character.setBottomCollision(gameMap.bottomCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, velocityY));
		character.setTopCollision(gameMap.topCollision(x, y, CHARACTER_WIDTH, 1));
	}
	else {					// Y速度向上
		character.setBottomCollision(gameMap.bottomCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1));
		character.setTopCollision(gameMap.topCollision(x, y, CHARACTER_WIDTH, -1*velocityY));
	}
		
	if (velocityX > 0) {
		character.setRightCollision(gameMap.rightCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, velocityX));
		character.setLeftCollision(gameMap.leftCollision(x, y, CHARACTER_HEIGHT, 1));
	}
	else {
		character.setRightCollision(gameMap.rightCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1));
		character.setLeftCollision(gameMap.leftCollision(x, y, CHARACTER_HEIGHT, (-1*velocityX)));
	}*/
}

void CGameStateRun::OnInit()  								// Game initial values and graphics settings
{
	load_background();
	character.init();
	load_object();
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP)
	{
		character.setMoveUp(true);
	}
	if (nChar == VK_DOWN)
	{
		character.setMoveDown(true);
	}
	if (nChar == VK_SPACE)
	{
		character.jumpCharge(true);
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP)
	{
		character.setMoveUp(false);
	}
	if (nChar == VK_DOWN)
	{
		character.setMoveDown(false);
	}
	if (nChar == VK_SPACE)
	{
		character.jumpCharge(false);
	}
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
	character.onShow();
	background.ShowBitmap();
	plane.SetTopLeft(character.getX(), character.getYshow());
	plane.ShowBitmap(1.5);
}

void CGameStateRun::load_background()
{
	background.LoadBitmap("Resources/Background.bmp");
	background.SetTopLeft(0, 0);
}

void CGameStateRun::load_object()
{
	plane.LoadBitmapByString({"Resources/Plane.bmp"}, RGB(0, 100, 0));
	plane.SetTopLeft(character.getX(), character.getYshow());
}