#include "stdafx.h"
#include "level3.h"

using namespace levels;

void level3::OnBeginState()
{
	levelInit::OnBeginState();

	accelerationConst = 0;
	pointSpeedDeficit = 0;
	obstacleSpeed = 0;
	time = 0;
	loadObject();
}

void level3::OnShow()
{
	levelInit::OnShow();

	for (int i = 0; i < obstacleNum; i++)
	{
		building[i].ShowBitmap();
		cloud[i].ShowBitmap();
		night.ShowBitmap();
	}

	for (int i = 0; i < obstacleNum; i++)
	{
		// Plane Crash Implementation
		if (game_framework::CMovingBitmap::IsOverlap(character, cloud[i]) || game_framework::CMovingBitmap::IsOverlap(character, building[i]))
		{
			if (character.isCheat() == true)
			{
				continue;
			}
			character.setCollide(true);
			pause = true;
			break;
		}
		// Add point if passes obstacle
		if (building[i].GetLeft() >= (character.GetLeft() - 128 - pointSpeedDeficit) && building[i].GetLeft() <= (character.GetLeft() - 126 + pointSpeedDeficit)) {
			point += 1;
		}
		// If already passed all obstacle
		if (point == obstacleNum)
		{
			congrats = true;
			pause = true;
			break;
		}
	}
	// Below pop-ups needs to be put here (on every level) so that the menu is shown on the very front

	// Game Over Pop Up
	if (character.isCollide() == true)
	{
		theMenu.ShowGameOver();
	}
	// Congrats Pop Up
	if (congrats == true)
	{
		theMenu.ShowCongrats();
	}
	// Pausing Game Pop Up
	if (pause == true && character.isCollide() == false && congrats == false)
	{
		theMenu.ShowGamePaused();
	}
}

void level3::OnMove()
{
	levelInit::OnMove();

	moveObstacle();
}


void level3::loadObject()
{
	for (int i = 0; i < obstacleNum; i++)
	{
		pathLocation[i] = (std::rand() % 20 + 5) * 20;
		pathHeight[i] = (std::rand() % 4 + 8) * 20;
		if (i > 0) {
			obstacleDistance[i] = abs(pathLocation[i - 1] - pathLocation[i]) - (pathHeight[i] / 10) + obstacleXDimension;
			if (accelerationConst != 0) {
				obstacleDistance[i] += i * 80;
			}
		}
		else {
			obstacleDistance[i] = 0;
		}

		building[i].LoadBitmapByString({ "Resources/Building1.bmp" }, RGB(0, 100, 0));
		building[i].SetTopLeft(xMax, yMax - pathLocation[i] + pathHeight[i] / 2);

		cloud[i].LoadBitmapByString({ "Resources/Cloud1.bmp" }, RGB(0, 100, 0));
		cloud[i].SetTopLeft(xMax, 0 - pathLocation[i] - pathHeight[i] / 2);
	}
	night.LoadBitmapByString({ "Resources/night.bmp" }, RGB(255, 255, 255));
}

void level3::moveObstacle()
{
	time += 1;
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
			if ((building[i - 1].GetLeft() + obstacleXDimension) <= xMax - obstacleDistance[i]) {
				cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop());
				building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop());
			}
		}

	}
}
