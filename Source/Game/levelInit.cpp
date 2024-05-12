﻿#include "stdafx.h"
#include "levelInit.h"

using namespace levels;

void levelInit::OnBeginState()
{
    point = 0;
    pointSpeedDeficit = 0;
    obstacleSpeed = 0;
    obstacleDistance = 1193;
    pathDifference = 0;
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

void levelInit::OnMove()
{
    if (isPause == false)
    {
        character.gravity();
        character.jump();
        moveObstacle();
    }
}

void levelInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
            isToInit = true;
        }
		
        else if (selectArrow.GetTop() == 400 && nChar == VK_RETURN) // Back to menu
        {
            selectArrow.SetTopLeft(390, 360);
            isToInit = true;
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
            isToInit = true;
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

bool levelInit::getToInit()
{
    return isToInit;
}


void levelInit::OnShow()
{
    background.ShowBitmap();

    character.ShowBitmap();

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
            // drawText("GAME OVER", 490, 320, 32, {255, 255, 255});
            // drawText("Try again", 540, 360, 24, {255, 255, 255});
            // drawText("Back to Main Menu", 490, 400, 24, {255, 255, 255});
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
        // drawText("Congratulations", 490, 320, 32, { 255, 255, 255 });
        // drawText("Next stage", 540, 360, 24, { 255, 255, 255 });
        // drawText("Back to Main Menu", 490, 400, 24, { 255, 255, 255 });
        selectArrow.ShowBitmap();
    }
}

void levelInit::load_object()
{
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

void levelInit::load_background()
{
    background.LoadBitmap("Resources/Background.bmp");
    background.SetTopLeft(0, 0);
}

void levelInit::moveObstacle()
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


std::vector<writeText> levelInit::getText()
{
    vector<writeText> texts;
    texts.push_back({"Altitude: " + to_string(670-character.GetTop()-80), {20, 50}, RGB(0, 0, 0), 20});
    texts.push_back({"Point: " + to_string(getPoint()), {20, 70}, RGB(0, 0, 0), 20});
    return texts;
}

int levelInit::getPoint()
{
    return point;
}