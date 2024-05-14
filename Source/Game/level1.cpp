#include "stdafx.h"
#include "level1.h"

using namespace levels;

void level1::OnBeginState()
{
    levelInit::OnBeginState();

    counter = 1;
    time = 0;
    pointSpeedDeficit = 0;
    obstacleSpeed = 0;
    obstacleDistance = 1193;
    pathDifference = 0;
    loadObject();
}

void level1::OnShow()
{
    levelInit::OnShow();
    
    for (int i = 0; i < obstacleNum; i++)
    {
        building[i].ShowBitmap();
        cloud[i].ShowBitmap();
    }
    
    for (int i = 0; i < obstacleNum; i++)
    {
        // Temp Overlap Implementation
        if (game_framework::CMovingBitmap::IsOverlap(character, cloud[i]) || game_framework::CMovingBitmap::IsOverlap(character, building[i]))
        {
            character.setCollide(true);
            pause = true;
        }
		
        if (building[i].GetLeft() >= (character.GetLeft()-128-pointSpeedDeficit) && building[i].GetLeft() <= (character.GetLeft() - 126+pointSpeedDeficit)) {
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

void level1::OnMove()
{
    levelInit::OnMove();

    moveObstacle();
}


void level1::loadObject()
{
    for (int i = 0; i < obstacleNum; i++)
    {
        pathLocation = (rand() % 20 + 5) * 20;
        pathHeight = (rand() % 4 + 8) * 20;
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

void level1::moveObstacle()
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
    for (int i = 0; i < counter; i++)
    {
        cloud[i].SetTopLeft(cloud[i].GetLeft() - obstacleMovementConst - obstacleSpeed, cloud[i].GetTop());
        building[i].SetTopLeft(building[i].GetLeft() - obstacleMovementConst - obstacleSpeed, building[i].GetTop());
    }
}

