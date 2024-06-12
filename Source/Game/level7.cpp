#include "stdafx.h"
#include "level7.h"

using namespace levels;

void level7::OnBeginState()
{
	levelInit::OnBeginState();

	gravityTimer = 180;
	accelerationConst = 0;
	pointSpeedDeficit = 0;
	obstacleSpeed = 0;
	time = 0;
	loadObject();
}

void level7::OnShow()
{
	levelInit::OnShow();

	for (int i = 0; i < obstacleNum; i++)
	{
		building[i].ShowBitmap();
		cloud[i].ShowBitmap();
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
	// Congrats Pop Up (Last Level)
	if (congrats == true)
	{
		endGame = true;
		theMenu.ShowEndMenu();
	}
	// Pausing Game Pop Up
	if (pause == true && character.isCollide() == false && congrats == false)
	{
		theMenu.ShowGamePaused();
	}
}

void level7::OnMove()
{
	levelInit::OnMove();

	moveObstacle();
	changeGravity();
}

void level7::loadObject()
{
	for (int i = 0; i < obstacleNum; i++)
	{
		pathLocation[i] = (std::rand() % 20 + 5) * 20 - 50;
		pathHeight[i] = (std::rand() % 4 + 8) * 20 + 100;
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
}

void level7::moveObstacle()
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

	for (int i = 0; i < obstacleNum; i++)
	{
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

void level7::changeGravity()
{
	gravityTimer -= 1;
	if (gravityTimer == 0)
	{
		if (character.isReverseGravity() == false)
		{
			character.setReverseGravity(true);
		}
		else
		{
			character.setReverseGravity(false);
		}
		gravityTimer = 180;
	}
}


std::vector<writeText> level7::getText(int r, int g, int b)
{
	vector<writeText> texts = levelInit::getText(r, g, b);

	if (character.isReverseGravity() == false)
	{
		texts.push_back({ "Current gravity   : normal", {20, 150}, RGB(r, g, b), 20 });
	}
	else
	{
		texts.push_back({ "Current gravity   : reverse", {20, 150}, RGB(r, g, b), 20 });
	}
	texts.push_back({ "Gravity change in : " + to_string(gravityTimer / 30), {20, 170}, RGB(r, g, b), 20 });

	return texts;
}