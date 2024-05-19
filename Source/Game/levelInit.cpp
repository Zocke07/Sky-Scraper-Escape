#include "stdafx.h"
#include "levelInit.h"

#include "level1.h"

using namespace levels;

void levelInit::OnBeginState()
{
    point = 0;
    pause = false;
    congrats = false;
    retry = false;
    character.init();
    loadBackground();
    character.load();
    theMenu.loadObject();
}

void levelInit::OnMove()
{
    character.movement();
}

void levelInit::OnInit()
{
    
}

void levelInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_ESCAPE)
    {
        if (this->isPause() == false) {
            pause = true;
        }
        else {
            pause = false;
        }
    }
    
    if (character.isCollide() == false && congrats == false)
    {
        character.OnKeyDown(nChar, nRepCnt, nFlags);
    }

    else if (character.isCollide() == true) // When character crashes
    {
        theMenu.OnKeyDownVertical(nChar, nRepCnt, nFlags);

        // If the user choose to go to main menu, will go to init
        toInit = theMenu.GameOverChoose(nChar, nRepCnt, nFlags);

        // If not, will retry level
        if (toInit == false  && nChar == VK_RETURN)
        {
            setRetry(true);
        }
    }

    if (congrats == true) // When character reaches target point
    {
        theMenu.OnKeyDownVertical(nChar, nRepCnt, nFlags);

        // If the user choose to go to main menu, will go to init
        toInit = theMenu.CongratsChoose(nChar, nRepCnt, nFlags);

        // If not, will go to next stage
        if (toInit == false && nChar == VK_RETURN)
        {
            setNextLevel(true);
        }
    }
}

void levelInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    
}

void levelInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    
}

void levelInit::OnLButtonUp(UINT nFlags, CPoint point)
{
    
}

void levelInit::OnMouseMove(UINT nFlags, CPoint point)
{
    
}

void levelInit::OnRButtonDown(UINT nFlags, CPoint point)
{
    
}

void levelInit::OnRButtonUp(UINT nFlags, CPoint point)
{
    
}

void levelInit::OnShow()
{
    background.ShowBitmap();
    character.ShowBitmap();
    
    if (character.isCollide() == true)
    {
        explosion.SetTopLeft(character.GetLeft() + 60, character.GetTop());
        explosion.ShowBitmap();
    }
    
    if (isPause() == true && character.isCollide() == false && congrats == false)
    {
        theMenu.ShowGamePaused();
    }
    
    if (congrats == true) {
        pause = true;
        theMenu.ShowCongrats();
    }
}


bool levelInit::isRetry()
{
    return retry;
}

bool levelInit::isPause()
{
    return pause;
}

bool levelInit::isToInit()
{
    return toInit;
}

bool levelInit::isNextlevel()
{
    return nextLevel;
}

std::vector<writeText> levelInit::getText()
{
    vector<writeText> texts;
    texts.push_back({"Altitude: " + to_string(670-character.GetTop()-80), {20, 50}, RGB(0, 0, 0), 20});
    texts.push_back({"Point   : " + to_string(point), {20, 70}, RGB(0, 0, 0), 20});
    return texts;
}

void levelInit::setToInit(bool flag)
{
    toInit = flag;
}

void levelInit::setRetry(bool flag)
{
    retry = flag;
}

void levelInit::setNextLevel(bool flag)
{
    nextLevel = flag;
}

    
void levelInit::loadBackground()
{
    background.LoadBitmap("Resources/Background.bmp");
    background.SetTopLeft(0, 0);

    explosion.LoadBitmapByString({"Resources/Explosion1.bmp"}, RGB(0, 100, 0));
}