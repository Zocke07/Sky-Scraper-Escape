#include "stdafx.h"
#include "level4.h"

using namespace levels;

void level4::OnBeginState()
{
	levelInit::OnBeginState();

	accelerationConst = 0;
	pointSpeedDeficit = 0;
	obstacleSpeed = 0;
	time = 0;
	loadObject();
}

void level4::OnShow()
{
	levelInit::OnShow();

	for (int i = 0; i < obstacleNum; i++)
	{
		building[i].ShowBitmap();
		cloud[i].ShowBitmap();
	}

	// Temp Overlap Implementation
	for (int i = 0; i < obstacleNum; i++)
	{
		if (game_framework::CMovingBitmap::IsOverlap(character, cloud[i]) || game_framework::CMovingBitmap::IsOverlap(character, building[i]))
		{
			character.setCollide(true);
			pause = true;
		}

		if (building[i].GetLeft() >= (character.GetLeft() - 128 - pointSpeedDeficit) && building[i].GetLeft() <= (character.GetLeft() - 126 + pointSpeedDeficit)) {
			point += 1;
		}

		// Congratulations Pop Up
		if (point == obstacleNum)
		{
			congrats = true;
			break;
		}

		// Game Over Pop Up (Needs to be put here so that the menu is shown on the very front)
		if (character.isCollide() == true)
		{
			theMenu.ShowGameOver();
		}
	}
}

void level4::OnMove()
{
	levelInit::OnMove();

	moveObstacle();
}


void level4::loadObject()
{
	for (int i = 0; i < obstacleNum; i++)
	{
		pathLocation[i] = (std::rand() % 20 + 5) * 20;
		//exclusive algorithm for level 4
		pathHeight[i] = (std::rand() % 4 + 8) * 24;
		//exclusive algorithm for level 4
		if (i > 0) {
			obstacleDistance[i] = abs(pathLocation[i - 1] - pathLocation[i]) + (pathHeight[i] / 2) + obstacleXDimension;
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

		//true= moveup, false= movedown
		if (i % 2 == 0) {
			obstacleDirection[i] = true;
		}
		else {
			obstacleDirection[i] = false;
		}
	}
}

void level4::moveObstacle()
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
			if (obstacleDirection[i] == true) {
				cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop() + 1);
				building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop() + 1);
				if (building[i].GetTop() == yMax - pathLocation[i] + pathHeight[i]) {
					obstacleDirection[i] = false;
				}
			}
			else {
				cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop() - 1);
				building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop() - 1);
				if (building[i].GetTop() == yMax - pathLocation[i]) {
					obstacleDirection[i] = true;
				}
			}
				
		}
		else {
			if ((building[i - 1].GetLeft() + obstacleXDimension) <= xMax - obstacleDistance[i]) {
				if (obstacleDirection[i] == true) {
					cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop() + 1);
					building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop() + 1);
					if (building[i].GetTop() == yMax - pathLocation[i] + pathHeight[i]) {
						obstacleDirection[i] = false;
					}
				}
				else {
					cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop() - 1);
					building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop() - 1);
					if (building[i].GetTop() == yMax - pathLocation[i]) {
						obstacleDirection[i] = true;
					}
				}
			}
		}

	}
}