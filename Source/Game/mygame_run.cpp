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
	pointSpeedDeficit = 0;
	obstacleSpeed = 0;
	isPause = false;
	congrats = false;
	selector = 1;
	counter = 1;
	time = 0;
	character.init();
	load_background();
	load_object();
	character.load();
}

void CGameStateRun::OnMove()							// Moving game element
{
	if (isPause == false)
	{

		character.gravity();
		character.jump();
		moveObstacle();
	}
}

void CGameStateRun::OnInit()  								// Game initial values and graphics settings
{
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (character.isCollide() == false)
	{
		if (nChar == VK_SPACE)
		{

			character.getCurrentJump();
			character.setJumping(true);
		}
	}

	else if (character.isCollide() == true) // When plane crashes
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

	character.ShowBitmap();

	for (int i = 0; i < obstacleNum; i++)
	{
		building[i].ShowBitmap();
		cloud[i].ShowBitmap();
	}


	drawText("Altitude:" + std::to_string(670-character.GetTop()-80), 20, 50, 20, {0, 0, 0});
	drawText("Point:" + std::to_string(point), 20, 70, 20, {0, 0, 0});

	// Temp Overlap Implementation
	for (int i = 0; i < obstacleNum; i++)
	{

		if (CMovingBitmap::IsOverlap(character, cloud[i]) || CMovingBitmap::IsOverlap(character, building[i]))
		{

			explosion.SetTopLeft(character.GetLeft() + 60, character.GetTop());
			explosion.ShowBitmap();
			// GotoGameState(GAME_STATE_OVER);

			character.setCollide(true);
			isPause = true;
		}
		
		if (building[i].GetLeft() >= (character.GetLeft()-128-pointSpeedDeficit) && building[i].GetLeft() <= (character.GetLeft() - 126+pointSpeedDeficit)) {
			point += 1;
		}

		// Game Over Menu

		if (character.isCollide() == true)
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

			break;
		}
	}
	if (congrats == true) {
		isPause = true;
		drawText("Congratulations", 490, 320, 32, { 255, 255, 255 });
		drawText("Next stage", 540, 360, 24, { 255, 255, 255 });
		drawText("Back to Main Menu", 490, 400, 24, { 255, 255, 255 });
		selectArrow.ShowBitmap();
	}
}

void CGameStateRun::load_background()
{
	background.LoadBitmap("Resources/Background.bmp");
	background.SetTopLeft(0, 0);
}

void CGameStateRun::load_object()
{
	explosion.LoadBitmapByString({"Resources/Explosion1.bmp"}, RGB(0, 100, 0));

	selectArrow.LoadBitmapByString({"Resources/SelectionArrow.bmp"}, RGB(0, 100, 0));
	selectArrow.SetTopLeft(390, 360);
	
	for (int i = 0; i < obstacleNum; i++)
	{
		pathLocation[i] = (std::rand() % 20 + 5) * 20;
		pathHeight[i] = (std::rand() % 4 + 8) * 20;
		if (i > 0) {
			obstacleDistance[i] = abs(pathLocation[i-1] - pathLocation[i]) - (pathHeight[i] / 10) + obstacleXDimension;
		}

		building[i].LoadBitmapByString({ "Resources/Building1.bmp" }, RGB(0, 100, 0));
		building[i].SetTopLeft(xMax, yMax - pathLocation[i] + pathHeight[i] / 2);

		cloud[i].LoadBitmapByString({ "Resources/Cloud1.bmp" }, RGB(0, 100, 0));
		cloud[i].SetTopLeft(xMax, 0 - pathLocation[i] - pathHeight[i] / 2);
	}
}

void CGameStateRun::moveObstacle()
{
	time += 1;
	if (time % 90 == 0 && counter < obstacleNum)
	{
		counter += 1;
	}
	if (time % 180 == 0)
	{
		obstacleSpeed += accelerationConst;
	}
	if (time % 330 == 0)
	{
		pointSpeedDeficit += accelerationConst;
	}

	for (int i = 0; i < obstacleNum; i++) {
		if (i == 0) {
			cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop());
			building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop());
		}
		else {
			if ((xMax - building[i-1].GetLeft()) >= obstacleDistance[i]) {
				cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop());
				building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop());
			}
		}

	}
}

void CGameStateRun::drawText(string text, int x, int y, int size, vector<int> rgbValue) {

	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, size, "·L³n¥¿¶ÂÅé", RGB(rgbValue[0], rgbValue[1], rgbValue[2]));
	CTextDraw::Print(pDC, x, y, text);
	CDDraw::ReleaseBackCDC();
}