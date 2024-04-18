#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <experimental/filesystem>

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
}

void CGameStateRun::OnMove()							// Moving game element
{
	if (isPause == false)
	{
		Gravity();
		Jump();
		moveObstacle();
	}
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
	if (nChar == VK_ESCAPE)
	{
		if (isPause == false) {
			isPause = true;
		}
		else {
			isPause = false;
		}
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

	for (int i = 0; i < obstacleNum; i++)
	{
		building[i].ShowBitmap();
		cloud[i].ShowBitmap();
	}

	drawText("Altitude:" + std::to_string(670-plane.GetTop()), 20, 50);
	drawText("Point:" + std::to_string(point), 20, 70);

	// Temp Overlap Implementation
	for (int i = 0; i < obstacleNum; i++)
	{
		if (CMovingBitmap::IsOverlap(plane, cloud[i]) || CMovingBitmap::IsOverlap(plane, building[i]))
		{
			explosion.SetTopLeft(plane.GetLeft() + 60, plane.GetTop());
			explosion.ShowBitmap();
			// GotoGameState(GAME_STATE_OVER);
			isPause = true;
		}
		if (building[i].GetLeft() >= (plane.GetLeft()-128) && building[i].GetLeft() <= (plane.GetLeft() - 126)) {
			point += 1;
		}
	}
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

	explosion.LoadBitmapByString({"Resources/Explosion1.bmp"}, RGB(0, 100, 0));

	for (int i = 0; i < obstacleNum; i++)
	{
		pathLocation = (std::rand() % 20 + 5) * 20;
		pathHeight = (std::rand() % 4 + 8) * 20;

		if (i > 0) {
			obstacleDistance += (pathLocation/3 +pathHeight/3);
		}
		building[i].LoadBitmapByString({"Resources/Building1.bmp"}, RGB(0, 100, 0));
		building[i].SetTopLeft(obstacleDistance, 652 - pathLocation + pathHeight/2);

		cloud[i].LoadBitmapByString({"Resources/Cloud1.bmp"}, RGB(0, 100, 0));
		cloud[i].SetTopLeft(obstacleDistance, 0 - pathLocation - pathHeight/2);

	}
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

void CGameStateRun::moveObstacle()
{
	time += 1;
	if (time % 90 == 0 && counter < obstacleNum)
	{
		counter += 1;
		time = 0; 
	}
	for (int i = 0; i < counter; i++)
	{
		cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst, cloud[i].GetTop());
		building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst, building[i].GetTop());
	}
}

void CGameStateRun::drawText(string text, int x, int y) {

	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 20, "·L³n¥¿¶ÂÅé", RGB(0, 0, 0));
	CTextDraw::Print(pDC, x, y, text);
	CDDraw::ReleaseBackCDC();
}