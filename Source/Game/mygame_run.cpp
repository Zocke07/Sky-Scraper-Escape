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
	
}

void CGameStateRun::OnInit()  								// Game initial values and graphics settings
{
	load_background();
	load_object();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT)
	{
		flying_object.SetTopLeft(flying_object.GetLeft()-30, flying_object.GetTop());
	}
	if (nChar == VK_RIGHT)
	{
		flying_object.SetTopLeft(flying_object.GetLeft()+30, flying_object.GetTop());
	}
	if (nChar == VK_UP)
	{
		flying_object.SetTopLeft(flying_object.GetLeft(), flying_object.GetTop()-30);
	}
	if (nChar == VK_DOWN)
	{
		flying_object.SetTopLeft(flying_object.GetLeft(), flying_object.GetTop()+30);
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
	flying_object.ShowBitmap();
}

void CGameStateRun::load_background()
{
	background.LoadBitmap("Resources/Background.bmp");
	background.SetTopLeft(0, 0);
}

void CGameStateRun::load_object()
{
	flying_object.LoadBitmap("Resources/Plane.bmp", RGB(0, 100, 0));
	flying_object.SetTopLeft(120, 280);
}