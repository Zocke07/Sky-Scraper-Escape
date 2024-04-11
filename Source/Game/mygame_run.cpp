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
	Gravity();
	Jump();
}

void CGameStateRun::OnInit()  								// Game initial values and graphics settings
{
	load_background();
	load_object();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE)
	{
		currentJump = plane.GetTop() - jumpConst;
		isJumping = true;
	}
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
	background.ShowBitmap();
	plane.ShowBitmap();
}

void CGameStateRun::load_background()
{
	background.LoadBitmap("Resources/Background.bmp");
	background.SetTopLeft(0, 0);
}

void CGameStateRun::load_object()
{
	plane.LoadBitmapByString({"Resources/Plane.bmp"}, RGB(0, 100, 0));
	plane.SetTopLeft(180, 120);
}

void CGameStateRun::Gravity()
{
	if (plane.GetTop() < 590 && isJumping == false) {
		plane.SetTopLeft(plane.GetLeft(), plane.GetTop() + gravityConst);
	}
}

void CGameStateRun::Jump()
{
	const int maxJumpHeight = 0;
	if (isJumping == true)
	{
		if (plane.GetTop() > maxJumpHeight && plane.GetTop() > currentJump)
		{
			plane.SetTopLeft(plane.GetLeft(), plane.GetTop() - gravityConst);
		}
		else
		{
			isJumping = false;
		}
	}
}
