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
	point = 0;
	obstacleDistance = 1193;
	pathDifference = 0;
	isPause = false;
	collide = false;
	congrats = false;
	selector = 1;
	counter = 1;
	time = 0;
	load_background();
	load_object();
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
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (collide == false)
	{
		if (nChar == VK_SPACE)
		{
			currentJump = plane.GetTop() - jumpConst;
			isJumping = true;
		}
	}
	else if (collide == true) // When plane crashes
	{
		if (nChar == VK_DOWN) // Move arrow down
		{
			if (selector < 2)
			{
				selector += 1;
				selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() + 40);
			}
		}
		else if (nChar == VK_UP) // Move arrow up
		{
			if (selector > 1) {
				selector -= 1;
				selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() - 40);
			}
		}
		
		if (selectArrow.GetTop() == 360 && nChar == VK_RETURN) // Try again
		{
			selectArrow.SetTopLeft(390, 360);
			GotoGameState(GAME_STATE_RUN);
		}
		
		else if (selectArrow.GetTop() == 400 && nChar == VK_RETURN) // Back to menu
		{
			selectArrow.SetTopLeft(390, 360);
			GotoGameState(GAME_STATE_INIT);
		}
	}
	if (congrats == true) // When plane reaches target point
	{
		if (nChar == VK_DOWN) // Move arrow down
		{
			if (selector < 2)
			{
				selector += 1;
				selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() + 40);
			}
		}
		else if (nChar == VK_UP) // Move arrow up
		{
			if (selector > 1) {
				selector -= 1;
				selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() - 40);
			}
		}
		if (selectArrow.GetTop() == 360 && nChar == VK_RETURN) // Go to next stage button
		{
			/*
			selectArrow.SetTopLeft(390, 360);
			GotoGameState(GAME_STATE_RUN);
			*/
		}
		
		else if (selectArrow.GetTop() == 400 && nChar == VK_RETURN) // Go to main menu button
		{
			selectArrow.SetTopLeft(390, 360);
			GotoGameState(GAME_STATE_INIT);
		}
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

	drawText("Altitude:" + std::to_string(670-plane.GetTop()-80), 20, 50, 20, {0, 0, 0});
	drawText("Point:" + std::to_string(point), 20, 70, 20, {0, 0, 0});

	// Temp Overlap Implementation
	for (int i = 0; i < obstacleNum; i++)
	{
		if (CMovingBitmap::IsOverlap(plane, cloud[i]) || CMovingBitmap::IsOverlap(plane, building[i]))
		{
			explosion.SetTopLeft(plane.GetLeft() + 60, plane.GetTop());
			explosion.ShowBitmap();
			// GotoGameState(GAME_STATE_OVER);
			collide = true;
			isPause = true;
		}
		
		if (building[i].GetLeft() >= (plane.GetLeft()-128) && building[i].GetLeft() <= (plane.GetLeft() - 126)) {
			point += 1;
		}

		// Game Over Menu
		if (collide == true)
		{
			drawText("GAME OVER", 490, 320, 32, {255, 255, 255});
			drawText("Try again", 540, 360, 24, {255, 255, 255});
			drawText("Back to Main Menu", 490, 400, 24, {255, 255, 255});
			selectArrow.ShowBitmap();
		}

		// Congratulations Pop Up
		if (point == obstacleNum)
		{
			congrats = true;
			drawText("Congratulations", 490, 320, 32, {255, 255, 255});
			drawText("Next stage", 540, 360, 24, {255, 255, 255});
			drawText("Back to Main Menu", 490, 400, 24, {255, 255, 255});
			selectArrow.ShowBitmap();
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
	plane.SetTopLeft(180, 270);

	explosion.LoadBitmapByString({"Resources/Explosion1.bmp"}, RGB(0, 100, 0));

	selectArrow.LoadBitmapByString({"Resources/SelectionArrow.bmp"}, RGB(0, 100, 0));
	selectArrow.SetTopLeft(390, 360);
	
	for (int i = 0; i < obstacleNum; i++)
	{
		pathLocation = (std::rand() % 20 + 5) * 20;
		pathHeight = (std::rand() % 4 + 8) * 20;
		pathDifference = abs(pathDifference - pathLocation);

		if (i > 0) {
			obstacleDistance += (pathDifference-pathHeight/10);
		}
		building[i].LoadBitmapByString({"Resources/Building1.bmp"}, RGB(0, 100, 0));
		building[i].SetTopLeft(obstacleDistance, 652 - pathLocation + pathHeight/2);

		cloud[i].LoadBitmapByString({"Resources/Cloud1.bmp"}, RGB(0, 100, 0));
		cloud[i].SetTopLeft(obstacleDistance, 0 - pathLocation - pathHeight/2);

		pathDifference = pathLocation;
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

void CGameStateRun::drawText(string text, int x, int y, int size, vector<int> rgbValue) {

	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, size, "�L�n������", RGB(rgbValue[0], rgbValue[1], rgbValue[2]));
	CTextDraw::Print(pDC, x, y, text);
	CDDraw::ReleaseBackCDC();
}